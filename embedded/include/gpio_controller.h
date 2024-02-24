/****************************** Module Header ******************************\
Module Name:    gpio_control.h
Project:        btb
Author:         Grant Maiden
Description:    Header file for gpio_control.cpp
\***************************************************************************/

/*  Some notes:
    Core Clock frequency is used to divide into SPI clock frequency.
    To find Core clock frequency, run in console:

    vcgencmd get_config int

    Then search for core_freq parameter.
*/

#define SPI_LED_BUF_LENGTH          444
#define SPI_LED_BUF_COLOR_INDEX     300
#define SPI_FLAGS                   0b000000000000000000000000011100000
#define SPI_FREQ                    7000000
#define SPI_CHANNEL                 0
#define LED_OUTPUT_HIGH             0b00001111
#define LED_OUTPUT_LOW              0b00000011

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

/**
 * Spew '1010' pattern to Leds using asm nop for delay
 * \param seconds- uint number of seconds to run test for
 * \returns bool- returns true if completed successfully or false if error occured.
 **/
bool ledNopAsmTest(uint seconds);

/**
 * Test command- sets led to RGB brightness level via SPI interface
 * \param array- char array 144 bits to be programmed to Leds
 * \returns bool- returns true if completed successfully or false if error occured.
 **/
bool gpioLedSpiTest(char *arr);

/**
 * Sends Spi Data at Baudrate required for leds.
 * \param buf- char *buf pointer to char buffer that contains spi data
  * \param length - unsigned length of data
 * \returns bool- returns true if completed successfully or false if error occured.
 **/
int gpioSpiSendData(char *buf, unsigned length);

/**
 * Sets Led Color Array to input 144bits (18bytes). Led<->Byte Order D1-D2-D3-D4-D5-D6
 * \param array- char array 144 bits to be programmed to Leds
 * \returns bool- returns true if completed successfully or false if error occured.
 **/
void gpioLedSetColor(char *arr);


