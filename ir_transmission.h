/** @file states.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Controls the IR communications of the game.

    Module the controls the IR communications for the game and 
    a few helper functions that decide who gets to start with the ball.
*/

#ifndef IR_TRANSMISSION_H
#define IR_TRANSMISSION_H

#include "projectile.h"
#include "ir_transmission.h"
#include "states.h"


/** Selects paper, scissors or rock to determine starting player
    @param selection Char address to store selected paper/scissors/rock*/
void select_PSR(char* selection);


/** Helper function to check for winner of paper, scissors, rock.
    @param selecion Own paper, etc. selection
    @param opponent Opponents selection
    @param restart Stays true unless winner is determined
    @param state Changes to WAITING for ball if lost, or BALL_SELECT if won */
void check_winner(char selection, char opponent, bool* restart, State* state);


/** Uses paper, scissors, rock to determine starting player
    @param state Game state to change depending on winner */
void starting_player_select(State* state);


/** Sends projectile data after converting appropriately for opponents display
    @param projectile Projectile object to send */
void send_projectile(Projectile* projectile);


/** Waits for projectile data from opponent. Allows nav_switch updates while waiting
    @param projectile Projectile object to store data to
    @param paddle Paddle object to move while waiting
    @param state Game state. Change to GAME_ON when data received */
void wait_for_data(Projectile* projectile, Paddle* paddle, State* state);



#endif //IR_TRANSMISSION_H