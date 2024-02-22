/****************************** Module Header ******************************\
Module Name:    gpio_control.h
Project:        btb
Author:         Grant Maiden
Description:    Header file for gpio_control.cpp
\***************************************************************************/

/**
 * GPIO Speed test. Toggles GPIO 4 (pin 7) at maximimum speed for user defined seconds.
 * \param seconds- uint number of seconds to run test for
 * \returns bool- returns true if completed successfully or false if error occured.
 **/
bool gpioTest(uint seconds);

/**
 * initialize PiGpio Library
 * \returns int- PiGpio library version
 **/
int initializePiGpio(void);

/**
 * Sets Output level of GPIO
 * \param GPIOnumber- gpio number that is being modified.
 * \param logicLevel- 0 or 1.
 * \returns int- PiGpio success or failure
 **/
int gpioSetOutput(int GPIOnumber, int logicLevel);


