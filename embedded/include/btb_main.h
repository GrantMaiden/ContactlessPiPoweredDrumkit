/****************************** Module Header ******************************\
Module Name:    btb_main.h
Project:        btb
Author:         Grant Maiden
Description:    Header file for btb.main.c
\***************************************************************************/

/**
 * Initialize Interrupts
 **/
inline void initInterrupts();

/**
 * Callback that is triggered on DistanceSensors Interrupt Falling Edge.
 * \param gpio - int gpio Triggered Gpio number
 * \param level - int level GPIO input level at time of ISR
 * \param tick - uint32_t trigger time in microseconds.
 **/
inline void rangingISRCallback(int gpio, int level, uint32_t tick);

/**
 * Initialize Leds
 **/
inline void initLeds();

/**
 * Parses Command line arguements. Command line inputs are used primarily for unit tests
 * \param argc - int containging the number of command line arguements
 * \param *argv[] - char* to an array of characters containing the input commands
 **/
inline void parseCommandLine(int argc,char *argv[]);

/**
 * Runs command line arguements
 * \param argv - char array input of characters containing the input command
 **/
inline void runCommandLine(char *argv[]);





