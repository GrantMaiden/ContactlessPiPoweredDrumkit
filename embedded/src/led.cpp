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

static ledStateMachine currentState = RESET_LEDS;
static ledStateMachine nextState = currentState;

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
Function Name:  initLeds()
Input Args:     none
Output Args:    none
Description:    intialize Leds globals
/**********************************************/
void initLeds()
{
    ledStateMachine currentState = ledStateMachine::INITIAL1;
    ledStateMachine nextState;
    int loops = 0;
    int loopWait = 0;
    int initialSpeed = INITIAL_LOOP_WAIT;
    char colorByte = 0x00;
    unsigned ColorRed = colorByte << 16;
    unsigned ColorGreen = colorByte << 8;
    unsigned ColorBlue = colorByte << 0;
    unsigned ColorYellow = ColorRed + ColorGreen;
    unsigned ColorCyan = ColorGreen + ColorBlue;
    unsigned ColorPurple = ColorRed + ColorBlue;
    unsigned ColourWhite = ColorRed + ColorGreen + ColorBlue;
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

/**********************************************\
Function Name:  sensorHit
Input Args:
Output Args:    void
Description:    Takes the hit information from the sensor logic and provides LED feeddback depending on the strength and starts loop timer for light
/**********************************************/

bool sensorHit(sensorID sensor, sensorValues)
{
    switch (sensor)
    {
    case sensorID::SENSOR1:
        int Sensor1Hit = 1;
        int Sensor1Str = sensorValues sens1Values.averageVelocity;
        int sensor1Timloop = 0;
        break;

    case sensorID::SENSOR2:
        Sensor2Hit = 1;
        Sensor2Str = Strength;
        sensor2Timloop = 0;
        break;

    case sensorID::SENSOR3:
        Sensor3Hit = 1;
        Sensor3Str = Strength;
        sensor3Timloop = 0;
        break;

    case sensorID::SENSOR4:
        Sensor4Hit = 1;
        Sensor4Str = Strength;
        sensor4Timloop = 0;
        break;

    case sensorID::SENSOR5:
        Sensor5Hit = 1;
        Sensor5Str = Strength;
        sensor5Timloop = 0;
        break;

    case sensorID::SENSOR6:
        Sensor6Hit = 1;
        Sensor6Str = Strength;
        sensor6Timloop = 0;
        break;
    }
}



/**********************************************\
Function Name:  sensorHitRecently
Input Args:
Output Args:    void
Description:    Checks if a sensor has been hit recently
/**********************************************/
bool sensorHitRecently()
{
    bool returnVal = false;
    if Sensor1Hit == 1
        returnVal = true;


    true;
}


/**********************************************\
Function Name:  ledSM
Input Args:     None
Output Args:    void
Description:    State Machine for LEDs
/**********************************************/

void ledSM()
{
    // Update current state from nextState
    //ledStateMachine currentState = ledStateMachine::INITIAL1;
    //ledStateMachine nextState;
    //currentState = nextState;
    char * colorArr = new char[18]();
    unsigned initialColour1 = 0x7;
    int loops = 0;
    int loopWait = 0;
    int initialSpeed = INITIAL_LOOP_WAIT;
    char colorByte = 0x00;

    initLeds();
    switch(currentState)
    {
        case RESET_LEDS:
            initLeds();
            nextState = INITIAL1;
            break;

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
                //initialSpeed = initialSpeed - 2;
                initialColour1 = initialColour1 * 5;
                loopWait = 0;
            }
            else if (loops == 3000)
            {
             nextState = PRIMARY1;
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

        case PRIMARY1:
            unsigned color1 = Sensor1Velocity  * BRIGHTNESS_LIMIT * LED_COLOR_PURPLE_DIM;
            unsigned color2 = Sensor2Velocity  * BRIGHTNESS_LIMIT * LED_COLOR_PURPLE_DIM;
            unsigned color3 = Sensor3Velocity  * BRIGHTNESS_LIMIT * LED_COLOR_PURPLE_DIM;
            unsigned color4 = Sensor4Velocity  * BRIGHTNESS_LIMIT * LED_COLOR_PURPLE_DIM;
            unsigned color5 = Sensor5Velocity  * BRIGHTNESS_LIMIT * LED_COLOR_PURPLE_DIM;
            unsigned color6 = Sensor6Velocity  * BRIGHTNESS_LIMIT * LED_COLOR_PURPLE_DIM;

            if sensorHitRecently()
            {
                hitDetectOutputControl(&color1 );
            }
            ledCreateColorArr(colorArr, color1, color2, color3, color4, color5, color6);
            gpioLedSetColor(colorArr);

            else
            {
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioLedSetColor(colorArr);
            }

            break;
//
//





    loopWait = loopWait+1;
    //sensor1Timloop = sensor1Timloop + 1;


// Turn Leds off
ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
gpioLedSetColor(colorArr);
printf("ledInitialiseTest Complete.\n");
}





/**********************************************\
Function Name:  ledInitialiseTest
Input Args:     None
Output Args:    void
Description:    Peter's LED test
/**********************************************/
/*
void ledInitialiseTest()
{
    printf("Beginning ledInitialiseTest. Runs Peter's LED Test Via SPI Interface\n");
    gpioInitializeLib();
    char * colorArr = new char[18]();
    ledStateMachine currentState = ledStateMachine::INITIAL1;
    unsigned initialColour1 = 0x7;
    ledStateMachine nextState;
    int loops = 0;
    int loopWait = 0;
    int initialSpeed = INITIAL_LOOP_WAIT;
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
                    //initialSpeed = initialSpeed - 2;
                    initialColour1 = initialColour1 * 5;
                    loopWait = 0;
                }
                else if (loops == 3000)
                {
                 nextState = PRIMARY1;
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
*/












