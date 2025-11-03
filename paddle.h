/** @file paddle.h
    @authors Kendrick Dela Cruz (kmd119),  Tio Sasanuma Howard (tsa95)
    @date 16 October 2025
    @brief Paddle object initialiser and driver
    
    This module initialises the paddles as well as dictates paddle 
    control. Paddles are drawn using the tinygl module. They are an object 
    composed of two points, top and bottom and a line is drawn between these 
    points forming a paddle. 
*/

#ifndef PADDLE_H
#define PADDLE_H

#include "tinygl.h"

//Defines a paddle with two edge co-ords
typedef struct {
    tinygl_point_t top_paddle;
    tinygl_point_t bottom_paddle;
} Paddle;


/** Returns a newly initialised paddle object 
    @return Paddle object */
Paddle init_paddle(void);


/** Displays the paddle in starting point 
    @param paddle Address of Paddle object. */
void display_paddle(Paddle* paddle);


/** Moves paddle up the screen one position 
    @param paddle Address of Paddle object. */
void paddle_up(Paddle* paddle);


/** Moves paddle down the screen one position 
    @param paddle Address of Paddle object. */
void paddle_down(Paddle* paddle);


/** Updates the paddle position in real-time 
    @param paddle Address of Paddle object. */
void update_paddle(Paddle* paddle);


/** Fetch the coordinates of the top of the paddle 
    @param paddle Address of Paddle object.
    @return y-coordinate of the top of the paddle.*/
tinygl_coord_t get_paddle_top(Paddle* paddle);


/** Fetch the coordinates of the top of the paddle 
    @param paddle Address of Paddle object.
    @return y-coordinate of the bottom of the paddle.*/
tinygl_coord_t get_paddle_bottom(Paddle* paddle);  

#endif 