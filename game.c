/** @file game.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 17 October 2025
    @brief Main game file for tennis!
*/

#include <stdint.h>
#include "paddle.h"
#include "tinygl.h"
#include "system.h"
#include "navswitch.h"
#include "projectile.h"
#include "pacer.h"
#include <avr/io.h>
#include "ledmat.h"
#include "../fonts/font5x7_1.h"
#include "states.h"
#include "ir_transmission.h"
#include "ir_serial.h"

#define PACER_RATE 500
#define UPDATE_RATE 800
#define BALL_START_POS {3, 3, 0, 1}

/** Initialises all modules */
void game_init(void)
{
    system_init ();
    tinygl_init(PACER_RATE);
    pacer_init(PACER_RATE);
    navswitch_init();
    ledmat_init();
    ir_serial_init();
}

/** Chooses action depending on game state
    @param state Current game state
    @param projectile Projectile to initialise/write data to
    @param paddle Paddle object to allow for updates during wait state */
void game_state(State* state, Projectile* projectile, Paddle* paddle)
{
    Start_Position position;
    switch (*state) {
        //Use paper, scissors, rock to determine starting player
        case BEGIN:
            press_to_start();
            pacer_wait();
            starting_player_select(state);
            break;
        
        //Starting player selects ball position
        case BALL_SELECT:
            position = choose_start(state);
            *projectile = projectile_init(position);
            break;

        //If opponent has ball, wait for ball
        case WAITING:
            wait_for_data(projectile, paddle, state);
            break;

        case GAME_ON:
            break;
    }
}

int main (void)
{
    //intialise modules and game variables
    game_init();
    State state = BEGIN;
    Paddle paddle = init_paddle();
    Projectile projectile = BALL_START_POS;
    uint16_t update_counter = 0;

    while (1)
    {   
        //Chooses what to do depending on game state
        game_state(&state, &projectile, &paddle);

        tinygl_clear();
        navswitch_update();

        /*update paddle and paddle display*/
        display_paddle(&paddle);
        update_paddle(&paddle);
        draw_projectile(&projectile);
        tinygl_update();

        /*Updates projectile every so often. When going straight
             updates 2x as fast as projectile is 2x as slow*/
        if ((update_counter % UPDATE_RATE == 0 || (update_counter % UPDATE_RATE/2 == 0 && projectile.delta_x == 0))) {
            update_pos(&projectile, &paddle, &state);
        } 
        update_counter++;
    } 
}
