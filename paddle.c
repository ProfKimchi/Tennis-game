/** @file paddle.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Paddle object initialiser and driver
*/

#include "tinygl.h"
#include "system.h"
#include "paddle.h"
#include "navswitch.h"
#include <avr/io.h>
#include <stdint.h>
#include "projectile.h"

/*Defines default paddle position and
    time between paddle updates*/
#define TOP_START_POINT tinygl_point(4, 4)
#define BOTTOM_START_POINT tinygl_point(4, 2)
#define PADDLE_TICKS 500
#define BOTTOM_SCREEN 0
#define TOP_SCREEN 4

/** Counts the number of program ticks elapsed. */
static uint64_t ticks = 0;

/** Returns a newly initialised paddle object 
    @return Paddle object */
Paddle init_paddle(void) 
{
    Paddle paddle;
    paddle.top_paddle = TOP_START_POINT;
    paddle.bottom_paddle = BOTTOM_START_POINT;
    return paddle;
}

/** Displays the paddle in starting point 
    @param paddle Address of Paddle object. */
void display_paddle(Paddle* paddle) 
{
    tinygl_draw_line(paddle->top_paddle, paddle -> bottom_paddle, 1);
}

/** Moves paddle up the screen one position 
    @param paddle Address of Paddle object. */
void paddle_up(Paddle* paddle) 
{
    if (paddle->bottom_paddle.y > BOTTOM_SCREEN) {
        paddle->top_paddle.y--;
        paddle->bottom_paddle.y--;
    }
}

/** Moves paddle down the screen one position 
    @param paddle Address of Paddle object. */
void paddle_down(Paddle* paddle) 
{
    if (paddle->bottom_paddle.y < TOP_SCREEN) {
        paddle->top_paddle.y++;
        paddle->bottom_paddle.y++;
    }
}

/** Updates the paddle position in real-time 
    @param paddle Address of Paddle object. */
void update_paddle(Paddle* paddle) 
{
    ticks++;
    if (ticks >= PADDLE_TICKS) {
        if (navswitch_down_p(NAVSWITCH_NORTH)) {
            paddle_up(paddle);
        } else if (navswitch_down_p(NAVSWITCH_SOUTH)) {
            paddle_down(paddle);
        }
        ticks = 0;
    }
}

/** Fetch the coordinates of the top of the paddle 
    @param paddle Address of Paddle object.
    @return y-coordinate of the top of the paddle.*/
tinygl_coord_t get_paddle_top(Paddle* paddle) 
{
    return paddle->top_paddle.y;
}

/** Fetch the coordinates of the top of the paddle 
    @param paddle Address of Paddle object.
    @return y-coordinate of the bottom of the paddle.*/
tinygl_coord_t get_paddle_bottom(Paddle*paddle) 
{
    return paddle->bottom_paddle.y;
}



