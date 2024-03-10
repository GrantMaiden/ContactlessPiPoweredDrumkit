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
Function Name:  ledFlash
Input Args:     ledColour, flashTimeOn, flashTimeOff, flashNum
Output Args:    void
Description:    Makes all LED flash an number of times for a given colour at a speed in seconds
/**********************************************/
void ledFlashTest(unsigned ledColour, int flashTimeOn, int flashTimeOff, int flashNum)

{
    char * colorArr = new char[18]();
    for (int i=0; i<flashNum; i++)
    {
        ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
        gpioLedSetColor(colorArr);
        sleep(flashTimeOff);

        ledCreateColorArr(colorArr, ledColour, ledColour, ledColour, ledColour, ledColour, ledColour);
        gpioLedSetColor(colorArr);
        sleep(flashTimeOn);
    }
    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
    gpioLedSetColor(colorArr);
}

/**********************************************\
Function Name:  ledFlash
Input Args:     ledColour,
Output Args:    void
Description:    Makes all LED fade to full brightness an number of times
/**********************************************/

// Timer needed
// Brightness incremented with timer count
// start from 0x00 - off end at max colour

void ledFadeTest(int fadeNum, int fadeSpeed)
{
    printf("Beginning ledFadeTest. Runs Peter's LED Test Via SPI Interface\n");
    struct timespec clockObj;
    clock_gettime(CLOCK_REALTIME, &clockObj);
    long int currentTime = clockObj.tv_sec;
    long int stopTime = fadeNum + clockObj.tv_sec;
    char colorByte = 0x00;
    int dir = 1;
    char * colorArr = new char[18]();
    while(stopTime>currentTime)
    {
        unsigned ColorRed = colorByte << 16;
        unsigned ColorGreen = colorByte << 8;
        unsigned ColorBlue = colorByte << 0;
        unsigned ColorYellow = ColorRed + ColorGreen;
        unsigned ColorCyan = ColorGreen + ColorBlue;
        unsigned ColorPurple = ColorRed + ColorBlue;
        unsigned ColourWhite = ColorRed + ColorGreen + ColorBlue;

        ledCreateColorArr(colorArr, ColorYellow, ColorYellow, ColorYellow, ColorYellow, ColorYellow, ColorYellow);
        gpioLedSetColor(colorArr);

        clock_gettime(CLOCK_REALTIME, &clockObj);
        currentTime = clockObj.tv_sec;

        for(long int i=0; i<fadeSpeed; i++){asm volatile("nop");}

        if(dir == 1)
        {
            colorByte = colorByte + dir;
            if (colorByte == 0xff)
            {
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                sleep(2);
            }
            else
            {
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
            }

        }
    }

    // Turn Leds off
    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
    gpioLedSetColor(colorArr);
}
//{
//    int ledFadeNum = 0;
//    char * colorArr = new char[18]();
//    char colorByte = 0x00;
//    unsigned ColorRed = colorByte << 16;
//    unsigned ColorGreen = colorByte << 8;
//    unsigned ColorBlue = colorByte << 0;
//    unsigned colorWhite = ColorRed + ColorGreen + ColorBlue;
//
//    {
//        while (ledFadeNum < 100)
//        {
//            ledCreateColorArr(colorArr, fadeColour, fadeColour, fadeColour, fadeColour, fadeColour, fadeColour);
//            gpioLedSetColor(colorArr);
//            colorByte = colorByte + 1;
//
//            if (colorByte == 0xff)
//            {
//                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
//                gpioLedSetColor(colorArr);
//            }
//        }
//    }
//}
/**********************************************\
Function Name:  hitStrengthFeedback
Input Args:
Output Args:    void
Description:    Takes the hit information from the sensor logic and provides LED feeddback depending on the strength
/**********************************************/

//void hitStrengthFeedback()
//{
//    if(velocity << xxx)
//    {
//
//
//    }
//
//    else if(velocity << xxx)
//    {
//
//    }
//
//}



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
    TSM nextState;
    int loops = 0;
    int loopWait = 0;
    int initialSpeed = INITIAL_SPEED;
    char colorByte = 0x00;
    unsigned ColorRed = colorByte << 16;
    unsigned ColorGreen = colorByte << 8;
    unsigned ColorBlue = colorByte << 0;
    unsigned ColorYellow = ColorRed + ColorGreen;
    unsigned ColorCyan = ColorGreen + ColorBlue;
    unsigned ColorPurple = ColorRed + ColorBlue;
    unsigned ColourWhite = ColorRed + ColorGreen + ColorBlue;


    while(loops <= 4000)
    {

        // Update current state from nextState
        currentState = nextState;
        switch(currentState)
        {
            case INITIAL1:
                ledCreateColorArr(colorArr, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == initialSpeed)
                {
                    nextState = INITIAL2;
                    loopWait = 0;
                }

                else if (loops == 3000)
                {
                 nextState = INITIAL7;
                 loopWait = 0;
                }
                break;

            case INITIAL2:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == initialSpeed)
                {
                    nextState = INITIAL3;
                    loopWait = 0;
                }

                else if (loops == 3000)
                {
                 nextState = INITIAL7;
                 loopWait = 0;
                }
                break;


            case INITIAL3:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == initialSpeed)
                {
                    nextState = INITIAL4;
                    loopWait = 0;
                }

                else if (loops == 3000)
                {
                 nextState = INITIAL7;
                 loopWait = 0;
                }
                break;

            case INITIAL4:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == initialSpeed)
                {
                    nextState = INITIAL5;
                    loopWait = 0;
                }

                else if (loops == 3000)
                {
                 nextState = INITIAL7;
                 loopWait = 0;
                }
                break;

            case INITIAL5:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait == initialSpeed)
                {
                    nextState = INITIAL6;
                    loopWait = 0;
                }

                else if (loops == 3000)
                {
                 nextState = INITIAL7;
                 loopWait = 0;
                }
                break;

            case INITIAL6:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1);
                gpioLedSetColor(colorArr);
                if (loopWait == initialSpeed)
                {
                    nextState = INITIAL1;
                    initialSpeed = initialSpeed - 2;
                    initialColour1 = initialColour1 * 5;
                    loopWait = 0;
                }
                else if (loops == 3000)
                {
                 nextState = INITIAL7;
                 loopWait = 0;
                }
                break;

            case INITIAL7:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (loopWait = 100)
                {
                    nextState = INITIAL8;
                    loopWait = 0;
                }
                break;

            case INITIAL8:
                //ledFlashTest(LED_COLOR_RED, 1, 1, 2);
                ledFadeTest(2, WAIT_5MS);
                nextState = INITIAL9;
                loopWait = 0;
                break;

            case INITIAL9:
                ledFlashTest(LED_COLOR_RED, 1, 1, 2);
                nextState = INITIAL10;
                loopWait = 0;
                break;

            case INITIAL10:
                ledFlashTest(LED_COLOR_GREEN, 2, 1, 1);
                nextState = INITIAL11;
                loopWait = 0;
                break;

            case INITIAL11:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                loopWait = 0;
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

/**********************************************\
Function Name:  mainLedFeedback
Input Args:     None
Output Args:    void
Description:    Runs Main Function LED Feedback. From a hit detection an LED will provide feedback depending on the strength of the hit.
/**********************************************/
/*
void mainLedFeedback()
{
    printf("Beginning mainLedFeedback. Runs Main Function LED Feedback Via SPI Interface\n");
    gpioInitializeLib();
    char * colorArr = new char[18]();
    LED_FEEDBACK currentState = LED_FEEDBACK::INITIALISE_FEEDBACK;
    LED_FEEDBACK nextState;

        while()
    {
        switch(currentState)
        {
            case INITIALISE_FEEDBACK:
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
                if (hitStrengthFeedback == 0)
                {
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM1_HARD:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_RED, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM1_MID:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_ORANGE_BRIGHT, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM1_SOFT:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_GREEN, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM2_HARD:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_RED, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM2_MID:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_ORANGE_BRIGHT, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM2_SOFT:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_GREEN, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM3_HARD:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_RED, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM3_MID:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_ORANGE_BRIGHT, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM3_SOFT:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_GREEN, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM4_HARD:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_RED, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM4_MID:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_ORANGE_BRIGHT, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM4_SOFT:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_GREEN, LED_COLOR_OFF, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM5_HARD:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_RED, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM5_MID:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_ORANGE_BRIGHT, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM5_SOFT:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_GREEN, LED_COLOR_OFF);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM6_HARD:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_RED);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM6_MID:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_ORANGE_BRIGHT);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;

            case DRUM6_SOFT:

                if ()
                {
                    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_GREEN);
                    gpioLedSetColor(colorArr);
                    nextState = INITIALISE_FEEDBACK;
                }
                break;


            default:
                nextState = INITIALISE_FEEDBACK;
                break;
        }
    }
}*/



