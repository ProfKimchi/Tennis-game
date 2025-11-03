/** @file states.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Controls IR communications of the game. 
*/

#include "ir_transmission.h"
#include "projectile.h"
#include "ir_serial.h"
#include "navswitch.h"
#include "paddle.h"
#include "tinygl.h"
#include "states.h"
#include "system.h"

/** Selects paper, scissors or rock to determine starting player
    @param selection Char address to store selected paper/scissors/rock*/
void select_PSR(char* selection)
{
    bool selected = 0;
    char choice_array[3] = {'R', 'P', 'S'};
    uint8_t i = 0;
    while (!selected) {
        navswitch_update();
        //de/in-crements the index by 1, looping around R/P/S
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            i = (i+2) % 3; 
        } else if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            i = (i+1) % 3;
        } else if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            //makes selection and sends to opponent
            selected = 1;
            *selection = choice_array[i];
            tinygl_clear();
        }
        //Displays character.
        char buffer[2] = {choice_array[i], '\0'};
        tinygl_text(buffer);
        tinygl_update();
    }

}

/** Helper function to check for winner of paper, scissors, rock.
    @param selecion Own paper, etc. selection
    @param opponent Opponents selection
    @param restart Stays true unless winner is determined
    @param state Changes to WAITING for ball if lost, or BALL_SELECT if won */
void check_winner(char selection, char opponent, bool* restart, State* state)
{
    switch (selection) {
        case 'R':
            switch (opponent) {
                case 'R':
                    //try again
                    break;
                case 'P':
                    *state = WAITING;
                    *restart = 0;
                    break;
                case 'S':
                    *state = BALL_SELECT;
                    *restart = 0;
                    break;
            }
            break;
        case 'P':
            switch (opponent) {
                case 'R':
                    *state = BALL_SELECT;
                    *restart = 0;
                    break;
                case 'P':
                    //try again
                    break;
                case 'S':
                    *state = WAITING;
                    *restart = 0;
                    break;
            }
            break;
        case 'S':
            switch (opponent) {
                case 'R':
                    *state = WAITING;
                    *restart = 0;
                    break;
                case 'P':
                    *state = BALL_SELECT;
                    *restart = 0;
                    break;
                case 'S':
                    //try again
                    break;
            } 
            break;
    }
}

/** Uses paper, scissors, rock to determine starting player
    @param state Game state to change depending on winner */
void starting_player_select(State* state)
{
    char selection = 'R'; //default is rock
    char opponent;
    bool restart = 1;
    bool received = 0;

    //Restarts selection loop until winner is determined
    while (restart) {
        received = 0;

        //Writes paper/scissors/rock to selection
        select_PSR(&selection);

        //Waits for push before sending selection
        navswitch_update();
        while (!navswitch_push_event_p(NAVSWITCH_PUSH)) {
            navswitch_update();

            //Receives opponent's selection if necessary
            if (!received) {
                received = ir_serial_receive((uint8_t*)(&opponent));
            }
        }
        ir_serial_transmit(selection);

        //If data not received, wait until it is
        while (!received) {
            received = ir_serial_receive((uint8_t*)(&opponent));
        }

        //Determine winner, then either restart, or change game state appropriately
        check_winner(selection, opponent, &restart, state);
    }
}

/** Sends projectile data after converting appropriately for opponents display
    @param projectile Projectile object to send */
void send_projectile(Projectile* projectile)
{
    //Convert projectile data for opponents display.
    projectile->x = 6 - projectile->x;
    projectile->delta_x *= -1;
    projectile->delta_y *= -1;

    //Transmits projectile data in order.
    ir_serial_transmit(projectile->x);
    ir_serial_transmit(projectile->y);
    ir_serial_transmit(projectile->delta_x);
    ir_serial_transmit(projectile->delta_y);
}

/** Waits for projectile data from opponent. Allows nav_switch updates while waiting
    @param projectile Projectile object to store data to
    @param paddle Paddle object to move while waiting
    @param state Game state. Change to GAME_ON when data received */
void wait_for_data(Projectile* projectile, Paddle* paddle, State* state)
{
    int8_t* address;
    for (int8_t i = 0; i < 4; i++) {
        //Specifies what projectile data should be received.
        switch (i) {
            case 0:
                address = &projectile->x;
                break;
            case 1:
                address = &projectile->y;
                break;
            case 2:
                address = &projectile->delta_x;
                break;
            case 3:
                address = &projectile->delta_y;
                break;
        }

        /*Waits for data to be written to specified location. 
            Also allows paddle updates while waiting*/
        while(ir_serial_receive((uint8_t*)address) != 1) {
            tinygl_clear();
            navswitch_update();
            display_paddle(paddle);
            update_paddle(paddle);
            tinygl_update();
        }
    }

    *state = GAME_ON;
}