/** @file states.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Projectile object initialiser and driver
    @note Co-ordinates in this file are transformed to make ball math more intuitive,
            e.g., (x,y) in here -> (4 - y, 6 - x) 
*/

#include <stdint.h>
#include "tinygl.h"
#include "ledmat.h"
#include "projectile.h"
#include "pacer.h"
#include "navswitch.h"
#include "paddle.h"
#include "states.h"
#include "ir_transmission.h"
#include "pacer.h"

#define MESSAGE_RATE 10
#define SAD_FACE_WAIT_TIME 15000
#define DEFAULT_X 3
#define DEFAULT_Y 0

/** Initialises the Projectile object.
    @param position The starting coordinates of the ball
    @return the projectile object. */
Projectile projectile_init(Start_Position position)
{
    Projectile projectile;
    switch (position) {
        /*Chooses from 1 of 7 pre-programmed directions, 
            depending on starting compass direction*/
        case WNW:
            projectile.delta_x = -2;
            projectile.delta_y = 1;
            break;
        case NW:
            projectile.delta_x = -1;
            projectile.delta_y = 1;
            break;
        case NNW:
            projectile.delta_x = -1;
            projectile.delta_y = 2;
            break;
        case N:
            projectile.delta_x = 0;
            projectile.delta_y = 1;
            break;
        case NNE:
            projectile.delta_x = 1;
            projectile.delta_y = 2;
            break;
        case NE:
            projectile.delta_x = 1;
            projectile.delta_y = 1;
            break;
        case ENE:
            projectile.delta_x = 2;
            projectile.delta_y = 1;
            break;
        default:
            projectile.delta_x = 0;
            projectile.delta_y = 1;
            break;
    }
    projectile.x = DEFAULT_X;
    projectile.y = DEFAULT_Y;
    return projectile;
}


/** Displays the ball at the specified coordinates.
    @param x x-coordinate of the ball
    @param y y-coordinate of the ball */
void display_point(int8_t x, int8_t y)
{
    ledmat_display_column((1<<(6 - x)), 4 - y);
}


/** Displays the ball at the chosen starting position.
    @param position The starting coordinates of the ball */
void display_start(Start_Position position)
{
    switch (position) {
        case WNW:
            display_point(1, 1);
            break;
        case NW:
            display_point(1, 2);
            break;
        case NNW:
            display_point(2, 2);
            break;
        case N:
            display_point(3, 3);
            break;
        case NNE:
            display_point(4, 2);
            break;
        case NE:
            display_point(5, 2);
            break;
        case ENE:
            display_point(5, 1);
            break;
        default:
            display_point(3, 3);
            break;
    }
}

/** Displays a sad face for the loser */
void display_sad(void)
{
    uint16_t update_counter = 0;
    //Sad face bit-map
    uint8_t column_array[5] = {0x0, 0x36, 0x0, 0x1C, 0x22};
    while (update_counter < SAD_FACE_WAIT_TIME) {
        for (uint8_t i = 0; i < 5; i++) {
            ledmat_display_column(column_array[i], i);
        }
        update_counter++;
    }
    ledmat_display_column(0x0, 4); //clears display
}


/** Chooses the starting position of the ball.
    @param state The current state of the game
    @return the chosen starting coordinates of the ball. */
Start_Position choose_start(State* state)
{
    Start_Position position = N;
    bool not_done = 1;
    while (not_done) {
        //displays the starting direction with a pixel at the start, and a pixel in the direction you can choose
        display_point(DEFAULT_X, DEFAULT_Y);
        display_start(position);

        //Cycles through start positions
        navswitch_update ();
        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && position != 0) {
            position -= 1;
            display_start(position);
        } else if (navswitch_push_event_p (NAVSWITCH_NORTH) && position != 6) {
            position += 1;
            display_start(position);
        } else if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            not_done = 0;
            *state = GAME_ON;
            return position;
        }
    }

    //Backup return statement
    *state = GAME_ON;
    return N;
}


/** Updates the position and checks for losing condition
    @param projectile the address of the ball/Projectile object
    @param paddle the address of the paddle object
    @param state the address of the current state of the game */
void update_pos(Projectile* projectile, Paddle* paddle, State* state)
{
    // Paddle collision check
    if (proj_in_paddle(projectile, paddle)) {
        projectile->y = 0;
        projectile->delta_y *= -1; // bounce upward

        // Adjust horizontal direction based on hit position
        tinygl_coord_t top = get_paddle_top(paddle);
        tinygl_coord_t bottom = get_paddle_bottom(paddle);
        tinygl_coord_t mid = (top + bottom) / 2;

        // If the ball hits near the top of the paddle, send it left
        if (projectile->x < 6 - mid)
            projectile->delta_x = -1;
        // If the ball hits near the bottom, send it right
        else if (projectile->x > 6 - mid)
            projectile->delta_x = 1;
        // Otherwise, retain angle
        else
            projectile->delta_x *= 1;
    }

    // Bottom edge (miss / lose). Returns to ball select for the loser.
    else if (projectile->y + projectile->delta_y < 0) {
        display_sad();
        pacer_wait();
        *state = BALL_SELECT;
    }

    // Top edge, sends projectile to opponent, then waits for ball
    else if (projectile->y + projectile->delta_y > 4) {
        *state = WAITING;
        send_projectile(projectile);
    }

    // Left/right wall collision
    if (projectile->x + projectile->delta_x < 0) {
        projectile->x = 0;
        projectile->delta_x *= -1;
    } else if (projectile->x + projectile->delta_x > 6) {
        projectile->x = 6;
        projectile->delta_x *= -1;
    }

    // Move
    projectile->x += projectile->delta_x;
    projectile->y += projectile->delta_y;
}


/** Checks if the ball has hit the paddle
    @param projectile the address of the ball/Projectile object
    @param paddle the address of the paddle object
    @return returns 1 if ball has hit paddle*/
bool proj_in_paddle(Projectile* projectile, Paddle* paddle)
{
    bool x_coords_align = (6 - get_paddle_top(paddle) <= projectile->x) && (projectile->x <= 6 - get_paddle_bottom(paddle));

    //makes sure paddle is hit, and is moving towards paddle
    return x_coords_align && (projectile->y + projectile->delta_y <= 0) && (projectile->delta_y < 0); 
}


/** Draws the position of the ball in real-time
    @param projectile address of the Projectile object. */
void draw_projectile(const Projectile* projectile)
{
    display_point(projectile->x, projectile->y);
}
