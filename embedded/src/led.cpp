
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

int sensor1Hit = 0;
int sensor2Hit = 0;
int sensor3Hit = 0;
int sensor4Hit = 0;
int sensor5Hit = 0;
int sensor6Hit = 0;

int sensor1Str = 0;
int sensor2Str = 0;
int sensor3Str = 0;
int sensor4Str = 0;
int sensor5Str = 0;
int sensor6Str = 0;

int sensor1Timloop = 0;
int sensor2Timloop = 0;
int sensor3Timloop = 0;
int sensor4Timloop = 0;
int sensor5Timloop = 0;
int sensor6Timloop = 0;

int loops = 0;
int loopWait = 0;

char * colorArr = new char[18]();

GpioController gpioController;
unsigned initialColour1 = 0x7;

/**********************************************\
Function Name:  ledCreateColorArr
Input Args:     outputArr, led1, led2, led3, led4, led5, led6
Output Args:    void
Description:    creates a single color array combined from input arguments. Will fill outputArr memory with combined data. Performs RGB->GRB byteshift
/**********************************************/
void LedControl::ledCreateColorArr(char* outputArr,unsigned led1, unsigned led2, unsigned led3, unsigned led4, unsigned led5, unsigned led6)
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
void LedControl::initLeds()
{
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
void LedControl::ledFlashTest(unsigned ledColour, int flashTimeOn, int flashTimeOff, int flashNum)
{
    char * colorArr = new char[18]();
    for (int i=0; i<flashNum; i++)
    {
        ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
        gpioController.gpioLedSetColor(colorArr);
        sleep(flashTimeOff);

        ledCreateColorArr(colorArr, ledColour, ledColour, ledColour, ledColour, ledColour, ledColour);
        gpioController.gpioLedSetColor(colorArr);
        sleep(flashTimeOn);
    }
    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
    gpioController.gpioLedSetColor(colorArr);
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

void LedControl::ledFadeTest(int fadeNum, int fadeSpeed)
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
        gpioController.gpioLedSetColor(colorArr);

        clock_gettime(CLOCK_REALTIME, &clockObj);
        currentTime = clockObj.tv_sec;

        for(long int i=0; i<fadeSpeed; i++){asm volatile("nop");}

        if(dir == 1)
        {
            colorByte = colorByte + dir;
            if (colorByte == 0xff)
            {
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioController.gpioLedSetColor(colorArr);
                sleep(2);
            }
            else
            {
                ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
                gpioController.gpioLedSetColor(colorArr);
            }

        }
    }

    // Turn Leds off
    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
    gpioController.gpioLedSetColor(colorArr);
}

/**********************************************\
Function Name:  sensorHitLed
Input Args:
Output Args:    void
Description:    Takes the hit information from the sensor controller and provides LED feeddback depending on the strength and starts loop timer for light
/**********************************************/

void LedControl::sensorHitLed(sensorID sensor, int velocity)
{
    int vel = velocity;
    switch (sensor)
    {
        case SENSOR1:
        {
            sensor1Hit = 1;
            sensor1Str = vel * ((vel > 0) - (vel < 0));
            sensor1Timloop = 0;
            break;
        }
        case SENSOR2:
        {
            sensor2Hit = 1;
            sensor2Str = vel * ((vel > 0) - (vel < 0));
            sensor2Timloop = 0;
            break;
        }
        case SENSOR3:
        {
            sensor3Hit = 1;
            sensor3Str = vel * ((vel > 0) - (vel < 0));
            sensor3Timloop = 0;
            break;
        }
        case SENSOR4:
        {
            sensor4Hit = 1;
            sensor4Str = vel * ((vel > 0) - (vel < 0));
            sensor4Timloop = 0;
            break;
        }
        case SENSOR5:
        {
            sensor5Hit = 1;
            sensor5Str = vel * ((vel > 0) - (vel < 0));
            sensor5Timloop = 0;
            break;
        }
        case SENSOR6:
        {
            sensor6Hit = 1;
            sensor6Str = vel * ((vel > 0) - (vel < 0));
            sensor6Timloop = 0;
            break;
        }
    }
}

/**********************************************\
Function Name:  sensorHitRecently
Input Args:
Output Args:    void
Description:    Checks if a sensor has been hit recently
/**********************************************/
bool LedControl::sensorHitRecently()
{
    bool returnVal = false;
    if (sensor1Hit == 1)
        returnVal = true;
    if (sensor2Hit == 1)
        returnVal = true;
    if (sensor3Hit == 1)
        returnVal = true;
    if (sensor4Hit == 1)
        returnVal = true;
    if (sensor5Hit == 1)
        returnVal = true;
    if (sensor6Hit == 1)
        returnVal = true;
}

/**********************************************\
Function Name:  hitDetectOutputControl
Input Args:
Output Args:    void
Description:    Generates color for a sensor for a given amount of time when hit
/**********************************************/
void LedControl::hitDetectOutputControl()
{
    if (sensor1Timloop > HIT_FLASH_DURATION)
        {
            sensor1Hit = 0;
        }
        if (sensor2Timloop > HIT_FLASH_DURATION)
        {
            sensor2Hit = 0;
        }
        if (sensor3Timloop > HIT_FLASH_DURATION)
        {
            sensor3Hit = 0;
        }
        if (sensor4Timloop > HIT_FLASH_DURATION)
        {
            sensor4Hit = 0;
        }
        if (sensor5Timloop > HIT_FLASH_DURATION)
        {
            sensor5Hit = 0;
        }
        if (sensor6Timloop > HIT_FLASH_DURATION)
        {
            sensor6Hit = 0;
        }
}
/**********************************************\
Function Name:  ledSM
Input Args:     None
Output Args:    void
Description:    State Machine for LEDs
/**********************************************/

void LedControl::ledSM()
{
    currentState = nextState;
    switch(currentState)
    {
        default:
            nextState = RESET_LEDS;
            break;

        case RESET_LEDS:
            ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
            gpioController.gpioLedSetColor(colorArr);
            nextState = INITIAL1;
            break;

        case INITIAL1:
            ledCreateColorArr(colorArr, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT)
            {
                nextState = INITIAL2;
                loopWait = 0;
            }

//            else if (loops >= 3000)
//            {
//             nextState = INITIAL7;
//             loopWait = 0;
//            }
            break;

        case INITIAL2:
            ledCreateColorArr(colorArr, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT)
            {
                nextState = INITIAL3;
                loopWait = 0;
            }

//            else if (loops >= 3000)
//            {
//             nextState = INITIAL7;
//             loopWait = 0;
//            }
            break;

        case INITIAL3:
            ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT/2)
            {
                nextState = INITIAL4;
                loopWait = 0;
            }

//            else if (loops >= 3000)
//            {
//             nextState = INITIAL7;
//             loopWait = 0;
//            }
            break;

        case INITIAL4:
            ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT/2)
            {
                nextState = INITIAL5;
                loopWait = 0;
            }

//            else if (loops >= 3000)
//            {
//             nextState = INITIAL7;
//             loopWait = 0;
//            }
            break;

        case INITIAL5:
            ledCreateColorArr(colorArr, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1, LED_COLOR_OFF);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT)
            {
                nextState = INITIAL6;
                loopWait = 0;
            }

//            else if (loops >= 3000)
//            {
//             nextState = INITIAL7;
//             loopWait = 0;
//            }
            break;

        case INITIAL6:
            ledCreateColorArr(colorArr, initialColour1, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, initialColour1);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT)
            {
                nextState = INITIAL7;
                //initialSpeed = initialSpeed - 2;
                initialColour1 = initialColour1 * 5;
                loopWait = 0;
            }
//            else if (loops >= 3000)
//            {
//             nextState = INITIAL7;
//             loopWait = 0;
//            }
            break;

        case INITIAL7:
            ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT)
            {
                nextState = INITIAL8;
                loopWait = 0;
            }
            break;

        case INITIAL8:
            ledFlashTest(LED_COLOR_RED, 1, 1, 2);
            //ledFadeTest(2, WAIT_5MS);
            nextState = INITIAL9;
            loopWait = 0;
            break;

        case INITIAL9:
            ledFlashTest(LED_COLOR_GREEN, 2, 1, 1);
            nextState = INITIAL10;
            loopWait = 0;
            break;

        case INITIAL10:
            ledCreateColorArr(colorArr, LED_COLOR_GREEN, LED_COLOR_GREEN, LED_COLOR_GREEN, LED_COLOR_GREEN, LED_COLOR_GREEN, LED_COLOR_GREEN);
            gpioController.gpioLedSetColor(colorArr);
            nextState = PRIMARY1;
            printf("LED intro over!\n");
            loopWait = 0;
            break;

        // Main drumming mode
        case PRIMARY1:
            //printf("Sensor 2 Strength: %i\n", sensor1Str);
            unsigned color1 = sensor1Str  * BRIGHTNESS_LIMIT * LED_COLOR_GREEN;
            unsigned color2 = sensor2Str  * BRIGHTNESS_LIMIT * LED_COLOR_GREEN;
            unsigned color3 = sensor3Str  * BRIGHTNESS_LIMIT * LED_COLOR_GREEN;
            unsigned color4 = sensor4Str  * BRIGHTNESS_LIMIT * LED_COLOR_GREEN;
            unsigned color5 = sensor5Str  * BRIGHTNESS_LIMIT * LED_COLOR_GREEN;
            unsigned color6 = sensor6Str  * BRIGHTNESS_LIMIT * LED_COLOR_GREEN;

            if (sensorHitRecently())
            {
                hitDetectOutputControl();
                color1 = sensor1Hit * LED_COLOR_BLUE_DIM;
                color2 = sensor2Hit * LED_COLOR_BLUE_DIM;
                color3 = sensor3Hit * LED_COLOR_BLUE_DIM;
                color4 = sensor4Hit * LED_COLOR_BLUE_DIM;
                color5 = sensor5Hit * LED_COLOR_BLUE_DIM;
                color6 = sensor6Hit * LED_COLOR_BLUE_DIM;
            }
            ledCreateColorArr(colorArr, color1, color2, color3, color4, color5, color6);
            gpioController.gpioLedSetColor(colorArr);
            break;

    sensor1Timloop = sensor1Timloop + 1;
    sensor2Timloop = sensor2Timloop + 1;
    sensor3Timloop = sensor3Timloop + 1;
    sensor4Timloop = sensor4Timloop + 1;
    sensor5Timloop = sensor5Timloop + 1;
    sensor6Timloop = sensor6Timloop + 1;

    // Turn Leds off
    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
    gpioController.gpioLedSetColor(colorArr);
    }
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
