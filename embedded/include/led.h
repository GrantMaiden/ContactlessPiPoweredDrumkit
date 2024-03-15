/****************************** Module Header ******************************\
Module Name:    led.h
Project:        btb
Author:         Grant Maiden
Description:    contains led definitions and headers
\***************************************************************************/
#ifndef LED_H
#define LED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>

#include "defines.h"
#include "gpio_controller.h"


// LED Colors
#define LED_COLOR_WHITE_DIM         0x010101
#define LED_COLOR_WHITE_MAX         0x111111
#define LED_COLOR_RED_DIM           0x010000
#define LED_COLOR_RED_MID           0x100000
#define LED_COLOR_RED               0x110000
#define LED_COLOR_GREEN_DIM         0x000100
#define LED_COLOR_GREEN_MID         0x001000
#define LED_COLOR_GREEN             0x001100
#define LED_COLOR_BLUE_DIM          0x000001
#define LED_COLOR_PURPLE_DIM        0x010001
#define LED_COLOR_YELLOW_DIM        0x010100
#define LED_COLOR_CYAN_DIM          0x000101
#define LED_COLOR_ORANGE_BRIGHT     0xCC8400
#define LED_COLOR_GREEN             0x001100
#define LED_COLOR_OFF               0x000000

//conversion factor (velocity-> brightness)
#define BRIGHTNESS_LIMIT            255/80

// LED Speed
#define INITIAL_LOOP_WAIT           200
#define HIT_FLASH_DURATION          60

typedef enum
{
    RESET_LEDS,
    INITIAL1,
    INITIAL2,
    INITIAL3,
    INITIAL4,
    INITIAL5,
    INITIAL6,
    INITIAL7,
    INITIAL8,
    INITIAL9,
    INITIAL10,
    PRIMARY1
}ledStateMachine;

using namespace std;
class LedControl
{
    public:

        /**
         * creates a single color array combined from input arguments. Will fill outputArr memory with combined data. Performs RGB->GRB byteshift
         * \param outputArr- char* this needs to be 18char long empty array
         * \param led1- unsigned 24bit value RGB color code.
         * \param led2- unsigned 24bit value RGB color code.
         * \param led3- unsigned 24bit value RGB color code.
         * \param led4- unsigned 24bit value RGB color code.
         * \param led5- unsigned 24bit value RGB color code.
         * \param led6- unsigned 24bit value RGB color code.
         **/
        void ledCreateColorArr(char* outputArr,unsigned led1, unsigned led2, unsigned led3, unsigned led4, unsigned led5, unsigned led6);

        /**
         * Initialize LED globals
         **/
        void initLeds();

        /**
         * Makes all LEDs flash an number of times for a given colour at a speed in seconds
         * \param ledColour-    unsigned 24bit value RGB color code.
         * \param flashTimeOn-  int number of seconds that LEDs are on
         * \param flashTimeOff- int number of seconds that LEDs are off
         * \param flashNum-     int number of times LEDs are flashed
         **/
        void ledFlashTest(unsigned ledColour, int flashTimeOn, int flashTimeOff, int flashNum);

        /**
         * Makes all LEDs Fade from (0 - 255) an number of times at a given speed
         * \param fadeStep-     int number of bits step changes in fade
         * \param fadeNum-      int number of times LEDs fade
         **/
        void ledFadeTest(int fadeNum, int fadeSpeed);


        /**
         * Takes the hit information from the sensor logic and provides LED feeddback depending on the strength
         * \param sensorID-     enum which sensor
         * \param velocity-     int current velocity measured by sensor
         **/
        void sensorHitLed(sensorID sensor, int velocity);

        /**
         * Checks if a sensor has been hit recently
         **/
        bool sensorHitRecently();

        /**
         * Generates color for a sensor for a given amount of time when hit
         *
         **/
        void hitDetectOutputControl();

        /**
         * State Machine for LEDs
         **/
        void ledSM();

        /**
         * Peter's LED Initializer test
         **/
        void ledInitialiseTest();

    private:

};



#endif // LED_H


