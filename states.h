/** @file states.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Controls the state of the game.
    
    This module controls the state of the game.
    We have modelled the game as a state machine with various states
    for the sake of modularity. This is essentially the "hub" and where
    all the state data and functions are kept. 
*/

#ifndef STATES_H
#define STATES_H
#include <stdint.h>

//Defines posiible game states
typedef enum {
    BEGIN,
    BALL_SELECT,
    WAITING,
    GAME_ON
} State;


/** While in the BEGIN state, the player is prompted
    via scrolling text to press the navswitch to start 
    the game.
    @param state address of the current state of the game. */
void press_to_start(void);

#endif