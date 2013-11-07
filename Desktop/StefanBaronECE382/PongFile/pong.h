/*
 * pong.h
 *
 *  Created on: Oct 16, 2013
 *      Author: C2C Stefan Baron von Vietinghoff-Scheel
 *      Function: The header file for the pong game, no visuals, declaring functions in the .c file
 */

#ifndef PONG_H_
#define PONG_H_


#define HEIGHT 750
#define WIDTH 750

typedef struct {
    int x;
    int y;
} vector2d_t;

typedef struct {
	unsigned char radius;
	vector2d_t position;
	vector2d_t velocity;
} ball_t;

ball_t createBall(int xPos, int yPos, int xVel, int yVel, unsigned char radius);

ball_t moveBall(ball_t ballToMove);



#endif /* MYPONG_H_ */
