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

/**********************************************\
Function Name:  initializePiGpio
Input Args:     void
Output Args:    int- PiGpio library version
Description:    initialize PiGpio
/**********************************************/
int initializePiGpio(void)
{
    gpioTerminate();
    int gpioVersion = gpioInitialise();
    if (gpioVersion > 0)
        printf("piGPIO Library initialized successfully. Version: %i\n",gpioVersion );
    else
    {
        printf("piGPIO library initalization FAILED\n");
        printf("result = %i\n", gpioVersion);
    }
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



