/* main.c - Lab 5 - A Simple Game
 *
 * Author: Stefan Baron von Vietinghoff-Scheel, USAF
 *
 * Function: Implements a function of the game. The game, a moving character
 * represented by an (*), will move across the 8 frame LCD screen on the
 * geek box in order to make it to the bottom right corner to essentially win
 * the game. In order to lose the game, the player takes either more than 2 seconds
 * for Required and "B" functionality or runs into a randomly placed mine (X) for "A"
 * functionality. Using push buttons 1-4, the user will be able to navigate across
 * the screen and avoid mines. The main.c file will include functions listed in
 * the header file as well as writing to the LCD screen. Strings are important
 * part of this file.
 *
 * Documentation: Day 1: I referenced Captain Branchflower's Code for help on getting
 * started with the code.
 */

#include <msp430.h>
#include "game.h"
#include "button.h"
#include "lcd.h"
#include "clkSpeed.h"

void init_timer();
void init_buttons();
void movingPlayer(char buttonToTest);
void Reset(char buttonToTest);

char btnPush = 0;
char timerCount = 0;
char player = 0;
char gameover = 0;

void clearTimer(){
        timerCount = 0;
        TACTL |= TACLR;
}

int main(void)
{
        WDTCTL = (WDTPW|WDTHOLD);


        player = initPlayer(); //debugging: had player defined twice, caused * to jump around alot
        initSPI();
        LCDinit();
        LCDclear();
        printPlayer(player);
        init_timer();
        init_buttons();
        __enable_interrupt();


        while(1)
        {
                 if(player == 0xC7){
                                    TACTL &= ~TAIE;
                                    LCDclear();
                                    cursorToLineOne();
                                    writeString("YOU");
                                    cursorToLineTwo();
                                    writeString("WON!");
                                    gameover = 1;
                                    _delay_cycles(100000);
                            }
                            if(timerCount >= 4){
                                    TACTL &= ~TAIE;
                                    LCDclear();
                                    cursorToLineOne();
                                    writeString("Game");
                                    cursorToLineTwo();
                                    writeString("Over!");
                                    gameover = 1;
                                    _delay_cycles(100000);
                            }
        }

        return 0;
}

//
// YOUR TIMER A ISR GOES HERE
//

void init_timer()
{
         TACTL &= ~(MC1|MC0);

                TACTL |= TACLR;
                setspeed_1MHz(); //got help from C2C Mossing for the setspeed code
                TACTL |= TASSEL1;
                TACTL |= ID1|ID0;
                TACTL &= ~TAIFG;
                TACTL |= MC1;
                TACTL |= TAIE;
}

void init_buttons()
{
       configureP2PinAsButton(BIT2);
       configureP2PinAsButton(BIT3);
       configureP2PinAsButton(BIT4);
       configureP2PinAsButton(BIT5);
       P2IES |= BIT2|BIT3|BIT4|BIT5;
       P2IFG &= ~ BIT2|BIT3|BIT4|BIT5;
       P2IE |= BIT2|BIT3|BIT4|BIT5;
}

void testAndRespondToButtonPush(char buttonToTest){
        if (buttonToTest & P2IFG){
                if(buttonToTest & P2IES){
                        movingPlayer(buttonToTest);
                        clearTimer();
                }
                else{
                        debounce();
                }
                P2IES ^=buttonToTest;
                P2IFG &= ~buttonToTest;
        }
}

void movingPlayer(char buttonToTest){ //C2C Mossing explained concept
        switch(buttonToTest){
        case BIT3:
                player=movePlayer(player,RIGHT);
                break;
        case BIT4:
                player=movePlayer(player, LEFT);
                break;
        case BIT5:
                player=movePlayer(player, UP);
                break;
        case BIT2:
                player=movePlayer(player, DOWN);
        }
}

void Reset(char buttonToTest){
        if(buttonToTest &P2IFG){
                if(buttonToTest & P2IES){
                        gameover=0;
                        LCDclear();
                        player=initPlayer();
                        printPlayer(player);
                        clearTimer();
                        TACTL|= TAIE;
                }
                else{
                        debounce();
                }
                P2IES ^= buttonToTest;
                P2IFG &= ~buttonToTest;
        }
}

#pragma vector= TIMER0_A1_VECTOR //C2C Mossing explained code for interrupts
__interrupt void TIMER0_A1_ISR(){
        TACTL &= ~TAIFG;
        timerCount++;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT_2_ISR(){
        if(gameover==0){
                testAndRespondToButtonPush(BIT2);
                testAndRespondToButtonPush(BIT3);
                testAndRespondToButtonPush(BIT4);
                testAndRespondToButtonPush(BIT5);
        }
        else{
                Reset(BIT2);
                Reset(BIT3);
                Reset(BIT4);
                Reset(BIT5);
        }
}

