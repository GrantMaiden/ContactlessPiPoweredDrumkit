/****************************** Module Header ******************************\
Module Name:    gpio_control.cpp
Project:        btb
Author:         Grant Maiden
Description:    Gpio Control functions and processes
\***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include <pigpio.h>

#include "defines.h"
#include "gpio_controller.h"
#include "led.h"

// Spi Globals
char * spiLedBuf = new char[SPI_LED_BUF_LENGTH]();
int spiHandle;

/**********************************************\
Function Name:  initializePiGpio
Input Args:     void
Output Args:    int- PiGpio library version
Description:    initialize PiGpio
/**********************************************/
int initializePiGpio(void)
{
    // init pins
    gpioTerminate();
    int gpioVersion = gpioInitialise();
    if (gpioVersion > 0)
        printf("piGPIO Library initialized successfully. Version: %i\n",gpioVersion );
    else
    {
        printf("piGPIO library initalization FAILED\n");
        printf("result = %i\n", gpioVersion);
    }

    // init spi buffer
    for (int i = 0; i<SPI_LED_BUF_LENGTH; i++)
    {
        spiLedBuf[i] = 0;
    }
    //spiLedBuf[SPI_LED_BUF_LENGTH - 2] = 0b11111111;
    //spiLedBuf[SPI_LED_BUF_LENGTH - 1] = 0b11111111;

    // Open Spi
    spiHandle = spiOpen(SPI_CHANNEL, SPI_FREQ, SPI_FLAGS);
    if (spiHandle < 0)
    {
        printf("spiHandle creation failed. returned: %i\n", spiHandle);
        return false;
    }


    return gpioVersion;
}

/**********************************************\
Function Name:  gpioTest
Input Args:     seconds- lenght to run
Output Args:    bool- completion status
Description:    toggle GPIO4 (pin7) as fast as possible
/**********************************************/
bool gpioTest(uint seconds)
{
    printf("Beginning gpioTest. gpioTest Toggles GPIO4 as fast as possible, testing frequency limitations of piGPIO library\n");
    initializePiGpio();
    if(gpioSetMode(GPIO4, PI_OUTPUT) != 0)
    {
        printf("failed to set GPIO mode on GPIO %i\n", GPIO4);
        return false;
    }
    sleep(1);
    struct timespec clockObj;
    clock_gettime(CLOCK_REALTIME, &clockObj);
    long int startTime = clockObj.tv_sec;
    long int currentTime = startTime;
    long int stopTime = seconds + startTime;
    printf("START TIME= %u\n", startTime);
    printf("STOP TIME= %u\n", stopTime);
    while (stopTime > currentTime)
    {
        gpioWrite(GPIO4, PI_HIGH);
        gpioWrite(GPIO4, PI_LOW);
        clock_gettime(CLOCK_REALTIME, &clockObj);
        currentTime = clockObj.tv_sec;
        //printf("STOP TIME= %u\n", currentTime);
    }
    printf("gpioTest Completed.\n");
    return true;
}

/**********************************************\
Function Name:  ledNopAsmTest
Input Args:     seconds- lenght to run
Output Args:    bool- completion status
Description:    Spew '1010' pattern to Leds using asm nop for delay
/**********************************************/
bool ledNopAsmTest(uint seconds)
{
    printf("Beginning ledNopAsmTest. Spew '1010' pattern to Leds using asm nop for delay\n");
    initializePiGpio();
    if(gpioSetMode(LED_SHIFT_3V3, PI_OUTPUT) != 0)
    {
        printf("failed to set GPIO mode on GPIO %i\n", GPIO4);
        return false;
    }
    sleep(1);
    struct timespec clockObj;
    clock_gettime(CLOCK_REALTIME, &clockObj);
    long int startTime = clockObj.tv_sec;
    long int currentTime = startTime;
    long int stopTime = seconds + startTime;
    printf("START TIME= %u\n", startTime);
    printf("STOP TIME= %u\n", stopTime);
    while (stopTime > currentTime)
    {

        gpioWrite(LED_SHIFT_3V3, PI_HIGH); // Takes 110ns to toggle pin
        for(uint i=0; i<WAIT_245NS; i++){asm volatile("nop");}
        for(uint i=0; i<WAIT_245NS; i++){asm volatile("nop");}
        gpioWrite(LED_SHIFT_3V3, PI_LOW); // Takes 110ns to toggle pin
        for(uint i=0; i<WAIT_245NS; i++){asm volatile("nop");}
        for(uint i=0; i<WAIT_245NS; i++){asm volatile("nop");}
        gpioWrite(LED_SHIFT_3V3, PI_HIGH); // Takes 110ns to toggle pin
        for(uint i=0; i<WAIT_190NS; i++){asm volatile("nop");}
        gpioWrite(LED_SHIFT_3V3, PI_LOW); // Takes 110ns to toggle pin
        for(uint i=0; i<WAIT_245NS; i++){asm volatile("nop");}
        for(uint i=0; i<WAIT_245NS; i++){asm volatile("nop");}
        for(uint i=0; i<WAIT_50NS; i++){asm volatile("nop");}
        for(uint i=0; i<WAIT_50NS; i++){asm volatile("nop");}
         //{ asm volatile("nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,nop,"); }

        clock_gettime(CLOCK_REALTIME, &clockObj); // this line and line below take roughly 300ns
        currentTime = clockObj.tv_sec;
        //printf("STOP TIME= %u\n", currentTime);
    }


    printf("ledNopAsmTest Completed.\n");
    return true;
}

/**********************************************\
Function Name:  gpioLedSpiTest
Input Args:     char array 144 bits to be programmed to Leds
Output Args:    bool- completion status
Description:    Test command- sets led RGB brightness level via SPI interface
/**********************************************/
bool gpioLedSpiTest(char *arr)
{
    printf("Beginning gpioLedSpiTest. sets led RGB brightness level via SPI interface\n");
    for (int l=0;l<18;l++)
    {
        printf("Led Color buffer index: %i\nValue: %i\n", l, arr[l]);
    }
    initializePiGpio();

    // blink test
    int loops = 0;
    while (loops <= 2)
    {
        printf("Generating Color Input stream and sending to LEDs\n");
        gpioLedSetColor(arr);
        gpioSpiSendData(spiLedBuf, SPI_LED_BUF_LENGTH);
        sleep(1);

        printf("Turning Leds off\n");
        for (int i = SPI_LED_BUF_COLOR_INDEX; i<SPI_LED_BUF_LENGTH; i++)
        {
            spiLedBuf[i] = LED_OUTPUT_LOW;
        }
        gpioSpiSendData(spiLedBuf, SPI_LED_BUF_LENGTH);
        sleep(1);

        loops++;
    }

    //Fade in then out
    struct timespec clockObj;
    clock_gettime(CLOCK_REALTIME, &clockObj);
    long int currentTime = clockObj.tv_sec;
    long int stopTime = GPIO_TEST_LENGTH_SEC + clockObj.tv_sec;
    char colorByte = 0x00;
    int dir = 1;
    while(stopTime>currentTime)
    {
        char * colorArr = new char[18]();
        unsigned ColorRed = colorByte << 16;
        unsigned ColorGreen = colorByte << 8;
        unsigned ColorBlue = colorByte << 0;
        ledCreateColorArr(arr, ColorRed, ColorGreen, ColorBlue, ColorRed, ColorGreen, ColorBlue);
        gpioLedSetColor(arr);
        gpioSpiSendData(spiLedBuf, SPI_LED_BUF_LENGTH);

        clock_gettime(CLOCK_REALTIME, &clockObj);
        currentTime = clockObj.tv_sec;
        for(long int i=0; i<WAIT_1MS; i++){asm volatile("nop");}

        if(dir == 1)
        {
            colorByte = colorByte + dir;
            if (colorByte == 0xff)
                dir = -1;
        }
        else
        {
            colorByte = colorByte + dir;
            if (colorByte == 0x0)
                dir = 1;
        }
    }



    printf("gpioLedSpiTest Completed.\n");

    return true;
}

/**********************************************\
Function Name:  gpioSpiSendData
Input Args:     char *buf
                unsigned length
Output Args:    int- PiGpio success or failure
Description:    Sends Data at Baudrate required for leds.
/**********************************************/
int gpioSpiSendData(char *buf, unsigned length)
{
    // Send Reset Command
    //gpioSetMode(LED_SHIFT_3V3, PI_OUTPUT);
    //gpioWrite(LED_SHIFT_3V3, PI_LOW);
    //for(uint i=0; i<WAIT_100US; i++){asm volatile("nop");}
    //for(uint i=0; i<WAIT_100US; i++){asm volatile("nop");}

    // Open Spi
    //spiHandle = spiOpen(SPI_CHANNEL, SPI_FREQ, SPI_FLAGS);
    //if (spiHandle < 0)
    //{
    //    printf("spiHandle creation failed. returned: %i\n", spiHandle);
    //    return false;
    //}

    // Send Write Data
    int spiWriteStatus = spiWrite(spiHandle, buf, length);
    if (spiWriteStatus < 0)
    {
        printf("Spi write failed. spiWrite() returned: %i\n", spiWriteStatus);
        return false;
    }

    // Close Spi Handle
    //int spiHandleClosedStatus = spiClose(spiHandle);// = spiClose(spiHandle);
    //if (spiHandleClosedStatus < 0)
    //{
    //    printf("Spi Handle close failed. spiClose() returned: %i\n", spiHandleClosedStatus);
    //    return false;
    //}
    return true;
}

/**********************************************\
Function Name:  gpioLedSetColor
Input Args:     char *arr[]
Output Args:    none
Description:    Sets Led Color Array to input 144bits (18bytes). Led<->Byte Order D1-D2-D3-D4-D5-D6
/**********************************************/
void gpioLedSetColor(char *arr)
{
    int index = 0;
    char outputByte = 0;
    for (int i=0;i<18;i++)
    {
        outputByte = arr[i];
        for (int j=7;j>=0;j--)
        {
            int shiftByte = outputByte >> j;
            if ((shiftByte & 0b1) == 0b1)
                spiLedBuf[SPI_LED_BUF_COLOR_INDEX + index] = LED_OUTPUT_HIGH;
            else
            {
                spiLedBuf[SPI_LED_BUF_COLOR_INDEX + index] = LED_OUTPUT_LOW;
                //printf("low @ index: %i\n", SPI_LED_BUF_COLOR_INDEX + index);
            }
            index = index+1;
        }
        //printf("Led Color buffer index: %i\nValue: %i\n", i, arr[i]);
    }


}

/**********************************************\
Function Name:  gpioSetOutput
Input Args:     GPIOnumber- gpio number that is being modified.
                logicLevel- 0 or 1.
Output Args:    int- PiGpio success or failure
Description:    Sets Output level of GPIO
/**********************************************/
int gpioSetOutput(int GPIOnumber, int logicLevel)
{
    return gpioWrite(GPIOnumber, logicLevel);
}



