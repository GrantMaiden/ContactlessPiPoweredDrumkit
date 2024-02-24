/****************************** Module Header ******************************\
Module Name:    led.h
Project:        btb
Author:         Grant Maiden
Description:    contains led definitions and headers
\***************************************************************************/

// LED Colors
#define LED_COLOR_WHITE_DIM 0x010101

/**
 * creates a single color array combined from input arguments. Will fill outputArr memory with combined data.
 * \param outputArr- char* this needs to be 18char long empty array
 * \param led1- unsigned 24bit value RGB color code.
 * \param led2- unsigned 24bit value RGB color code.
 * \param led3- unsigned 24bit value RGB color code.
 * \param led4- unsigned 24bit value RGB color code.
 * \param led5- unsigned 24bit value RGB color code.
 * \param led6- unsigned 24bit value RGB color code.
 **/
void ledCreateColorArr(char* outputArr,unsigned led1, unsigned led2, unsigned led3, unsigned led4, unsigned led5, unsigned led6);


