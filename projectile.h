/** @file states.c
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Projectile object initialiser and driver

    This module controls the behaviour of the ball. There is
    an initialiser for the Projectile object and functions tracking
    the position of the ball in relation to the screen and paddle objects.
    There are also functions dictating the movement of the ball
    by manipulation of its position. 
*/

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdint.h>
#include "paddle.h"
#include "states.h"

//Defines a pojectile with (x,y) coords, and change in (x, y) per update
typedef struct {
    int8_t x;
    int8_t y;
    int8_t delta_x;
    int8_t delta_y;
} Projectile;

//Defines starting positions for projectile with compass positions
typedef enum {
    WNW = 0, NW, NNW, N, NNE, NE, ENE
} Start_Position;


/** Initialises the Projectile object.
    @param position The starting coordinates of the ball
    @return the projectile object. */
Projectile projectile_init(Start_Position position);


/** Displays the ball at the specified coordinates.
    @param x x-coordinate of the ball
    @param y y-coordinate of the ball */
void display_point(int8_t x, int8_t y);


/** Displays the ball at the chosen starting position.
    @param position The starting coordinates of the ball */
void display_start(Start_Position position);

void display_sad(void);


/** Chooses the starting position of the ball.
    @param state The current state of the game
    @return the chosen starting coordinates of the ball. */
Start_Position choose_start(State* state);


/** Updates the position and checks for losing condition
    @param projectile the address of the ball/Projectile object
    @param paddle the address of the paddle object
    @param state the address of the current state of the game */
void update_pos(Projectile* projectile, Paddle* paddle, State* state);


/** Checks if the ball has hit the paddle
    @param projectile the address of the ball/Projectile object
    @param paddle the address of the paddle object
    @return returns 1 if ball has hit paddle*/
bool proj_in_paddle(Projectile* projectile, Paddle* paddle);


/** Draws the position of the ball in real-time
    @param projectile address of the Projectile object. */
void draw_projectile(const Projectile* projectile);

#endif //PROJECTILE_H