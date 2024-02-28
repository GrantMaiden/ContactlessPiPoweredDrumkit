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
#define LED_COLOR_GREEN_DIM         0x000100
#define LED_COLOR_BLUE_DIM          0x000001
#define LED_COLOR_PURPLE_DIM        0x010001
#define LED_COLOR_YELLOW_DIM        0x010100
#define LED_COLOR_CYAN_DIM          0x000101
#define LED_COLOR_ORANGE_BRIGHT     0xCC8400
#define LED_COLOR_RED               0x110000
#define LED_COLOR_GREEN             0x001100
#define LED_COLOR_OFF               0x000000

// LED Speed
#define INITIAL_SPEED               46

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
    INITIAL9
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
 * Peter's LED test
 **/
void ledInitialiseTest();
