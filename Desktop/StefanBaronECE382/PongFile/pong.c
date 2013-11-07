/* Pong
 * Author: C2C Stefan Baron von Vietinghoff-Scheel
 * Date: 16 Oct 2013
 * Description: source file of the the pong game code, no visuals
 */

#include <msp430.h>
#include "pong.h"

/*
 * main.c
 */
int main(int argc, const char *argv[]) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    ball_t ball = createBall(400, 420, 30, 40, 10);

        while (1)
        {
            printf("Ball x:%d, y:%d\n", ball.position.x, ball.position.y);
            sleep(1);
            ball = moveBall(ball);
        }
        return 0;
}
