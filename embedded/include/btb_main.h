/****************************** Module Header ******************************\
Module Name:    btb_main.h
Project:        btb
Author:         Grant Maiden
Description:    Header file for btb.main.c
\***************************************************************************/

extern "C" {
#include "VL53L4CD_api.h"
#include "platform.h"
#include "types.h"
}

/**
 * Runs Example 1 from STM32 Sensor Demo
 * \param dev Dev_t objec that contains sensor identifier and i2c address
 * \returns status enum of sensor
 **/
int example1(Dev_t dev);

/**
 * Initialize Distance Sensors
 **/
void initDistanceSensors();

/**
 * Initialize Leds
 **/
void initLeds();

/**
 * Initialize Interrupts
 **/
void initInterrupts();

/**
 * Parses Command line arguements. Command line inputs are used primarily for unit tests
 * \param argc - int containging the number of command line arguements
 * \param *argv[] - char* to an array of characters containing the input commands
 **/
void parseCommandLine(int argc,char *argv[]);

/**
 * Runs command line arguements
 * \param argv - char array input of characters containing the input command
 **/
void runCommandLine(char *argv[]);

