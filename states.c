/** @file states.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Controls the state of the game.
*/


#include "tinygl.h"
#include "states.h"
#include "navswitch.h"
#include "pacer.h"
#include "../fonts/font5x7_1.h"

#define TEXT_SPEED 20

/** While in the BEGIN state, the player is prompted
    via scrolling text to press the navswitch to start 
    the game.
    @param state address of the current state of the game. */
void press_to_start(void)
{
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(TEXT_SPEED);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("PRESS TO START");

    while(!navswitch_push_event_p(NAVSWITCH_PUSH)) {
        pacer_wait();
        tinygl_update();
        navswitch_update();
    }
}
