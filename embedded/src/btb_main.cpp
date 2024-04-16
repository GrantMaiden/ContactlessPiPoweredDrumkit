/****************************** Module Header ******************************\
Module Name:    btb_main.cpp
Project:        btb
Author:         Grant Maiden
Description:    Entry Point into ENG5220 project for University of Glasgow
\***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#include "btb_main.h"
#include "gpio_controller.h"
#include "defines.h"
#include "led.h"
#include "ranging.h"
#include "controller.h"
#include "sound.h"

// global variables
bool enableLedSM = false;
bool enableControllerSM = false;

volatile int sens1DataReady = 0;
volatile int sens2DataReady = 0;
volatile int sens3DataReady = 0;
volatile int sens4DataReady = 0;
volatile int sens5DataReady = 0;
volatile int sens6DataReady = 0;

/**********************************************\
Class Name:     VL53L4CD_test
Description:    Test class used for input test command line processing
/**********************************************/
class VL53L4CD_test : public VL53L4CD
{
    virtual void distanceDataReady(sensorValues sensVals, sensorID sensor)
    {
        printf("Data Received from Sensor: %i. Current Distance: %i \n",sensor + 1, sensVals.currentDistance_mm);
    }
};

VL53L4CD_test    vl53l4cd;

/**********************************************\
Function Name:  main
Input Args:     none
Output Args:    none
Description:    main function/application entry
/**********************************************/
int main(int argc, char *argv[])
{
	//// Parse Unit Tests and Other Command Line Args ////
	parseCommandLine(argc, argv);

    GpioController gpiocontroller;
    gpiocontroller.gpioInitializeLib();

    // initialize Controller, which handles the start of sensor ranging and led control.
    Controller controller;
    controller.initialize();

    printf("Press ctrl+z to end the BTB program\n");
    while(1)
    {
    }

}


/**********************************************\
Function Name:  parseCommandLine()
Input Args:     int argc        arguement count
                char *argv[]    arguement values
Output Args:    none
Description:    Parses Command line arguements. Command line inputs are used primarily for unit tests.
/**********************************************/
void parseCommandLine(int argc,char *argv[])
{
    if (argc > 1)
    {
        printf("Found %i additional input arguments\n\n", argc-1);
        for(int i=1; i<argc; i++)
        {
            printf("Attempting to run Command: %s\n", argv[i]);
            runCommandLine(&argv[i]);
        }
        exit(0);
    }
}

/**********************************************\
Function Name:  runCommandLine()
Input Args:     string arg   arguement values
Output Args:    none
Description:    Runs Command Line arguements
/**********************************************/
void runCommandLine(char *argv[])
{
    if(!strcmp(argv[0], "gpioTest"))
    {
        GpioController gpiocontroller;
        gpiocontroller.gpioTest(GPIO_TEST_LENGTH_SEC);
    }
    else if (!strcmp(argv[0], "test"))
    {
        printf("Test Argument Received!!\n");
    }
    else if (!strcmp(argv[0], "ledNopAsmTest"))
    {
        GpioController gpiocontroller;
        gpiocontroller.ledNopAsmTest(GPIO_TEST_LENGTH_SEC);
    }
    else if (!strcmp(argv[0], "gpioLedSpiTest"))
    {
        GpioController gpiocontroller;
        LedControl ledControl;
        char * arr = new char[18]();
        ledControl.ledCreateColorArr(arr, LED_COLOR_CYAN_DIM, LED_COLOR_PURPLE_DIM, LED_COLOR_YELLOW_DIM, LED_COLOR_WHITE_DIM, LED_COLOR_BLUE_DIM, LED_COLOR_GREEN_DIM);
        gpiocontroller.gpioLedSpiTest(arr);
    }
    else if (!strcmp(argv[0], "ledInitialiseTest"))
    {
        LedControl ledControl;
        ledControl.ledFadeTest(5, 20);
    }
    else if (!strcmp(argv[0], "interruptTest"))
    {
        GpioController gpiocontroller;
        gpiocontroller.gpioInitializeLib();

        // initialize Distance Sensors
        vl53l4cd.rangingInit();

        while (1)
        {
        }
    }
    else if (!strcmp(argv[0], "soundTest1"))
    {
        Sound sound;
        sound.soundTest1();
    }
    else if (!strcmp(argv[0], "soundTest2"))
    {
        Sound sound;
        sound.soundTest2();
    }
    else if (!strcmp(argv[0], "soundTest3"))
    {
        Sound sound;
        sound.soundTest3();
    }
    else if (!strcmp(argv[0], "rangingTestDistanceSensors"))
    {
        vl53l4cd.rangingTestDistanceSensors();
    }
    else
    {
        printf("Command: %s Failed to run!!!\n", argv[0]);
    }
}


