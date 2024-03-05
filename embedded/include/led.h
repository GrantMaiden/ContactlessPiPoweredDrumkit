/****************************** Module Header ******************************\
Module Name:    led.h
Project:        btb
Author:         Grant Maiden
Description:    contains led definitions and headers
\***************************************************************************/

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

// LED Speed
#define INITIAL_SPEED               45


enum TSM:int
{
    INITIAL1,
    INITIAL2,
    INITIAL3,
    INITIAL4,
    INITIAL5,
    INITIAL6,
    INITIAL7,
    INITIAL8,
    INITIAL9,
    INITIAL10
};

enum LED_FEEDBACK:int
{
    INITIALISE_FEEDBACK,
    DRUM1_HARD,
    DRUM1_MID,
    DRUM1_SOFT,
    DRUM2_HARD,
    DRUM2_MID,
    DRUM2_SOFT,
    DRUM3_HARD,
    DRUM3_MID,
    DRUM3_SOFT,
    DRUM4_HARD,
    DRUM4_MID,
    DRUM4_SOFT,
    DRUM5_HARD,
    DRUM5_MID,
    DRUM5_SOFT,
    DRUM6_HARD,
    DRUM6_MID,
    DRUM6_SOFT,
};

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
 * Makes all LED flash an number of times for a given colour at a speed in seconds
 * \param ledColour-    unsigned 24bit value RGB color code.
 * \param flashTimeOn-  int number of seconds that LEDs are on
 * \param flashTimeOff- int number of seconds that LEDs are off
 * \param flashNum-     int number of times LEDs are flashed
 **/
void ledFlash(unsigned ledColour, int flashTimeOn, int flashTimeOff, int flashNum);


/**
 * Takes the hit information from the sensor logic and provides LED feeddback depending on the strength
 *
 **/
//void hitStrengthFeedback()

/**
 * Peter's LED test
 **/
void ledInitialiseTest();

/**
 * Runs Main Function LED Feedback. From a hit detection an LED will provide feedback depending on the strength of the hit.
 **/
void mainLedFeedback();
