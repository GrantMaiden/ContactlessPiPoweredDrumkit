/****************************** Module Header ******************************\
Module Name:    led.cpp
Project:        btb
Author:         Grant Maiden
Description:    led control functions and processes
\***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>

#include "defines.h"
#include "gpio_controller.h"
#include "led.h"


/**********************************************\
Function Name:  ledCreateColorArr
Input Args:     outputArr, led1, led2, led3, led4, led5, led6
Output Args:    void
Description:    creates a single color array combined from input arguments. Will fill outputArr memory with combined data. Performs RGB->GRB byteshift
/**********************************************/
void ledCreateColorArr(char* outputArr,unsigned led1, unsigned led2, unsigned led3, unsigned led4, unsigned led5, unsigned led6)
{
    unsigned arr[6] = {led1,led2,led3,led4,led5,led6};

    int j= 0;
    for(int i=0;i<18;i=i+3)
    {
        outputArr[i]    = (arr[j] >> 8) & 0xff; //g
        outputArr[i+1]  = (arr[j] >> 16) & 0xff; //r
        outputArr[i+2]  = (arr[j]) & 0xff; //b
        j = j+1;
        //printf("low @ index: %i\n", i);
        //i = i + 2;
    }

}


/**********************************************\
Function Name:  ledInitialiseTest
Input Args:     None
Output Args:    void
Description:    Peter's LED test
/**********************************************/

void ledInitialiseTest()
{
    printf("Beginning ledInitialiseTest. Runs Peter's LED Test Via SPI Interface\n");
    gpioInitializeLib();
    char * colorArr = new char[18]();
    TSM currentState = TSM::INITIAL1;
    unsigned initialColour1 = 0x7;
    TSM nextState = currentState;
    int loops = 0;
    int loopWait = 0;

    while(loops <= 2000)
    {
        // Update current state from nextState
        currentState = nextState;
        switch(currentState)
        {
            case INITIAL1:
                ledCreateColorArr(colorArr, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == INITIAL_SPEED)
                {
                    nextState = INITIAL2;
                    loopWait = 0;
                }
                break;

            case INITIAL2:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == INITIAL_SPEED)
                {
                    nextState = INITIAL3;
                    loopWait = 0;
                }
                break;

            case INITIAL3:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == INITIAL_SPEED)
                {
                    nextState = INITIAL4;
                    loopWait = 0;
                }
                break;

            case INITIAL4:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == INITIAL_SPEED)
                {
                    nextState = INITIAL5;
                    loopWait = 0;
                }
                break;

            case INITIAL5:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == INITIAL_SPEED)
                {
                    nextState = INITIAL6;
                    loopWait = 0;
                }
                break;

            case INITIAL6:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1);
                gpioLedSetColor(colorArr);
                if (loopWait == INITIAL_SPEED)
                {
                    nextState = INITIAL1;
                    initialColour1 = initialColour1 * 5;
                    loopWait = 0;
                }
                //else if (loops == 1000 )
                //{
                // nextState = primaryState1
                //}
                break;

            default:
                nextState = INITIAL1;
                break;
        }



        // Prepare for next Iteration of SM
        loops = loops+1;
        loopWait = loopWait+1;

        // Simulate Delay
        gpioDelay(2500);
    }
    // Turn Leds off
    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
    gpioLedSetColor(colorArr);
    printf("ledInitialiseTest Complete.\n");

}




