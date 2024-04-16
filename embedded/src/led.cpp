
/****************************** Module Header ******************************\
Module Name:    led.cpp
Project:        btb
Author:         Grant Maiden and Lucas Zehner
Description:    led control functions and processes
\***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>
#include <CppThread.h>

#include "CppThread.h"
#include "CppTimer.h"

#include "defines.h"
#include "gpio_controller.h"
#include "led.h"

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

unsigned colorSet = LED_COLOR_OFF;
unsigned color1 = LED_COLOR_OFF;
unsigned color2 = LED_COLOR_OFF;
unsigned color3 = LED_COLOR_OFF;
unsigned color4 = LED_COLOR_OFF;
unsigned color5 = LED_COLOR_OFF;
unsigned color6 = LED_COLOR_OFF;

int loops = 0;
int loopWait = 0;
int sensor1VelConfirm = 0;
int sensor2VelConfirm = 0;
int sensor3VelConfirm = 0;
int sensor4VelConfirm = 0;
int sensor5VelConfirm = 0;
int sensor6VelConfirm = 0;

char * colorArr = new char[18]();
unsigned initialColour1 = 0x7;

static ledStateMachine currentState = RESET_LEDS;
static ledStateMachine nextState = currentState;
GpioController gpioController;


/**********************************************\
Function Name:  btbTimer1::timerEvent
Input Args:     none
Output Args:    none
Description:    override timerEvent from btbTimer1 class. Enables LED statemachine periodically.
/**********************************************/
void LedControl::timerEvent(){
    ledSM();
}

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

    //// Initialize Timers ////
	startns(BTB_TIMER_1_INTERVAL_NS);

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
Function Name:  setLedVelocity
Input Args:     sensorID sensor-    enum which sensor
                avgVelocity-        int current avgerage velocity measured by sensor
Output Args:    void
Description:    Takes filtered velocity from controller and gives it to led state machine
/**********************************************/
void LedControl::setLedVelocity(sensorID sensor, int avgVelocity)
{
    if (-2 < avgVelocity < 2)
        avgVelocity = 0;
        sensor1VelConfirm = 0;
        sensor2VelConfirm = 0;
        sensor3VelConfirm = 0;
        sensor4VelConfirm = 0;
        sensor5VelConfirm = 0;
        sensor6VelConfirm = 0;

    switch (sensor)
    {
        case SENSOR1:
        {
            sensor1Str = avgVelocity;
            sensor1VelConfirm = 1;
            break;
        }
        case SENSOR2:
        {
            sensor2Str = avgVelocity;
            sensor2VelConfirm = 1;
            break;
        }
        case SENSOR3:
        {
            sensor3Str = avgVelocity;
            sensor3VelConfirm = 1;
            break;
        }
        case SENSOR4:
        {
            sensor4Str = avgVelocity;
            sensor4VelConfirm = 1;
            break;
        }
        case SENSOR5:
        {
            sensor5Str = avgVelocity;
            sensor5VelConfirm = 1;
            break;
        }
        case SENSOR6:
        {
            sensor6Str = avgVelocity;
            sensor6VelConfirm = 1;
            break;
        }
    }
}

/**********************************************\
Function Name:  sensorHitLed
Input Args:     sensorID sensor-    enum which sensor
                hitStrength-        int hit strength velocity measured by sensor
Output Args:    void
Description:    Takes the hit information from the sensor controller and provides LED feeddback depending on the strength and starts loop timer for light
/**********************************************/
void LedControl::sensorHitLed(sensorID sensor, int hitStrength)
{
    switch (sensor)
    {
        case SENSOR1:
        {
            sensor1Hit = 1;
            sensor1Str = hitStrength;
            sensor1Timloop = 0;
            break;
        }
        case SENSOR2:
        {
            sensor2Hit = 1;
            sensor2Str = hitStrength;
            sensor2Timloop = 0;
            break;
        }
        case SENSOR3:
        {
            sensor3Hit = 1;
            sensor3Str = hitStrength;
            sensor3Timloop = 0;
            break;
        }
        case SENSOR4:
        {
            sensor4Hit = 1;
            sensor4Str = hitStrength;
            sensor4Timloop = 0;
            break;
        }
        case SENSOR5:
        {
            sensor5Hit = 1;
            sensor5Str = hitStrength;
            sensor5Timloop = 0;
            break;
        }
        case SENSOR6:
        {
            sensor6Hit = 1;
            sensor6Str = hitStrength;
            sensor6Timloop = 0;
            break;
        }
    }
}

/**********************************************\
Function Name:  sensorHitRecently
Input Args:     None
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
    return returnVal;
}

/**********************************************\
Function Name:  hitColorSetter
Input Args:     sensorStr-        int hit strength velocity measured by sensor
Output Args:    void
Description:    Generates color for a sensor for a given hit strength
/**********************************************/
unsigned LedControl::hitColorSetter(int sensorStr)
{
    if (sensorStr > HIT_HARD)
        colorSet = FLASH_COLOR_HARD;
    else if (sensorStr > HIT_MID_HARD)
        colorSet = FLASH_COLOR_MID_HARD;
    else if (sensorStr > HIT_MID)
        colorSet = FLASH_COLOR_MID;
    else if (sensorStr > HIT_MID_SOFT)
        colorSet = FLASH_COLOR_MID_SOFT;
    else if (sensorStr > HIT_SOFT)
        colorSet = FLASH_COLOR_SOFT;
    return colorSet;
}

/**********************************************\
Function Name:  hitDetectOutputControl
Input Args:     None
Output Args:    void
Description:    Generates color for a sensor for a given amount of time when hit
/**********************************************/
void LedControl::hitDetectOutputControl()
{
    if (sensor1Timloop < HIT_FLASH_DURATION & sensor1Hit == 1)
        {
            sensor1Timloop = sensor1Timloop +1;
            color1 = hitColorSetter(sensor1Str);
        }
    else
        {
            sensor1Hit = 0;
            sensor1Timloop = 0;
        }

    if (sensor2Timloop < HIT_FLASH_DURATION & sensor2Hit == 1)
        {
            sensor2Timloop = sensor2Timloop +1;
            color2 = hitColorSetter(sensor2Str);
        }
    else
        {
        sensor2Hit = 0;
        sensor2Timloop = 0;
        }

    if (sensor3Timloop < HIT_FLASH_DURATION & sensor3Hit == 1)
        {
        sensor3Timloop = sensor3Timloop +1;
        color3 = hitColorSetter(sensor3Str);
        }
    else
        {
        sensor3Hit = 0;
        sensor3Timloop = 0;
        }

    if (sensor4Timloop < HIT_FLASH_DURATION & sensor4Hit == 1)
        {
        sensor4Timloop = sensor4Timloop +1;
        color4 = hitColorSetter(sensor4Str);
        }
    else
        {
        sensor4Hit = 0;
        sensor4Timloop = 0;
        }

    if (sensor5Timloop < HIT_FLASH_DURATION & sensor5Hit == 1)
        {
        sensor5Timloop = sensor5Timloop +1;
        color5 = hitColorSetter(sensor5Str);
        }
    else
        {
        sensor5Hit = 0;
        sensor5Timloop = 0;
        }

    if (sensor6Timloop < HIT_FLASH_DURATION & sensor6Hit == 1)
        {
        sensor6Timloop = sensor6Timloop +1;
        color6 = hitColorSetter(sensor6Str);
        }
    else
        {
        sensor6Hit = 0;
        sensor6Timloop = 0;
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

            break;

        case INITIAL7:
            ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
            gpioController.gpioLedSetColor(colorArr);
            loopWait = loopWait+1;
            if (loopWait >= INITIAL_LOOP_WAIT)
            {
                nextState = INITIAL9;  //CHANGE TO INITIAL8 FOR COMPLETE LED INTRO
                loopWait = 0;
            }
            break;

        case INITIAL8:
            ledFlashTest(LED_COLOR_RED, 1, 1, 2);
            ledFadeTest(2, WAIT_5MS);
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
            // Uncomment lines to change LED lights from constantly on, to only turning on when hit
            color1 = LED_COLOR_BLUE_DIM;//* sensor1VelConfirm;
            color2 = LED_COLOR_BLUE_DIM;//* sensor2VelConfirm;
            color3 = LED_COLOR_BLUE_DIM;//* sensor3VelConfirm;
            color4 = LED_COLOR_BLUE_DIM;//* sensor4VelConfirm;
            color5 = LED_COLOR_BLUE_DIM;//* sensor5VelConfirm;
            color6 = LED_COLOR_BLUE_DIM;//* sensor6VelConfirm;
            if (sensorHitRecently())
            {
                hitDetectOutputControl();
            }
            ledCreateColorArr(colorArr, color1, color2, color3, color4, color5, color6);
            gpioController.gpioLedSetColor(colorArr);
            break;


    // Turn Leds off
    ledCreateColorArr(colorArr, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF, LED_COLOR_OFF);
    gpioController.gpioLedSetColor(colorArr);
    }
}


