/****************************** Module Header ******************************\
Module Name:    btb_main.cpp
Project:        btb
Author:         Grant Maiden
Description:    Entry Point into ENG5228 project for University of Glasgow
\***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <CppThread.h>

#include "CppThread.h"
#include "CppTimer.h"
#include "btb_main.h"
#include "gpio_controller.h"
#include "defines.h"
#include "led.h"
#include "controller.h"
#include "sound.h"


// global variables
bool enableLedSM = false;
bool enableControllerSM = false;

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

	//// Initialize Application ////
	gpioInitializeLib();
	//initialize Distance Sensors
	initInterrupts();
	initLeds();

    //// Initialize threads ////
	btbThread btbThread1;
	btbThread1.start();

    //// Initialize Timers ////
	btbTimer1 btbTimer_2p5ms;
	btbTimer_2p5ms.startns(BTB_TIMER_1_INTERVAL_NS);

}

/**********************************************\
Function Name:  btbThread::run
Input Args:     none
Output Args:    none
Description:    override virtual run method for btbThread Class. Calls Statemachines for output control
/**********************************************/
void btbThread::run() {
    while(1)
    {
        if(enableLedSM)
        {
            // TODO: call led state machine
            enableLedSM = false;
        }

        // controller Statemachine
        controllerSM();

        //printf("threadRunning\n");
    }
}

/**********************************************\
Function Name:  btbTimer1::timerEvent
Input Args:     none
Output Args:    none
Description:    override timerEvent from btbTimer1 class. Enables LED statemachine periodically.
/**********************************************/
void btbTimer1::timerEvent(){
    enableLedSM = true;
    //printf("TimerRunningCB\n");
}

/**********************************************\
Function Name:  initLeds()
Input Args:     none
Output Args:    none
Description:    intialize Leds
/**********************************************/
void initLeds()
{

}

/**********************************************\
Function Name:  rangingISRCallback()
Input Args:     int gpio :Triggered Gpio number
                int level: GPIO input level at time of ISR
                uint32_t tick: trigger time in microseconds.
Output Args:    none
Description:    callback that is triggered on DistanceSensors Interrupt Falling Edge.
/**********************************************/
void rangingISRCallback(int gpio, int level, uint32_t tick)
{
    //printf("GPIO %d became %d at %d\n", gpio, level, tick); //COMMENT ME OUT WHEN WORKING

    if (level == PI_TIMEOUT)
    {
        printf("GPIO %d returned Interrupt Timeout! Interrupt Exceeded %dms!\nUs tick: %lu\n", gpio, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, tick);
    }

    // LUCAS ETHAN ADD CODE to return senseValue struct.
    sensorValues senseValues;
    switch(gpio)
    {
        case D1_GPIO1:
            // TODO: ADD CODE senseValue = someFUNCTION()
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR1);
            break;
        case D2_GPIO1:
            // TODO: ADD CODE senseValue = someFUNCTION()
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR2);
            break;
        case D3_GPIO1:
            // TODO: ADD CODE senseValue = someFUNCTION()
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR3);
            break;
        case D4_GPIO1:
            // TODO: ADD CODE senseValue = someFUNCTION()
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR4);
            break;
        case D5_GPIO1:
            // TODO: ADD CODE senseValue = someFUNCTION()
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR5);
            break;
        case D6_GPIO1:
            // TODO: ADD CODE senseValue = someFUNCTION()
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR6);
            break;

    }
}

/**********************************************\
Function Name:  initInterrupts()
Input Args:     none
Output Args:    none
Description:    intialize interrupts and timers
/**********************************************/
void initInterrupts()
{
    gpioSetISRFunc(D1_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
    gpioSetISRFunc(D2_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
    gpioSetISRFunc(D3_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
    gpioSetISRFunc(D4_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
    gpioSetISRFunc(D5_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
    gpioSetISRFunc(D6_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
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
        gpioTest(GPIO_TEST_LENGTH_SEC);
    }
    else if (!strcmp(argv[0], "test"))
    {
        printf("Test Argument Received!!\n");
    }
    else if (!strcmp(argv[0], "ledNopAsmTest"))
    {
        ledNopAsmTest(GPIO_TEST_LENGTH_SEC);
    }
    else if (!strcmp(argv[0], "gpioLedSpiTest"))
    {
        char * arr = new char[18]();
        ledCreateColorArr(arr, LED_COLOR_CYAN_DIM, LED_COLOR_PURPLE_DIM, LED_COLOR_YELLOW_DIM, LED_COLOR_WHITE_DIM, LED_COLOR_BLUE_DIM, LED_COLOR_GREEN_DIM);
        gpioLedSpiTest(arr);
    }
    else if (!strcmp(argv[0], "ledInitialiseTest"))
    {
        ledInitialiseTest();
    }
    else if (!strcmp(argv[0], "interruptTest"))
    {
        gpioInitializeLib();
        // initialize Distance Sensors
        initInterrupts();
        sleep(10);
    }
    else if (!strcmp(argv[0], "soundTest1"))
    {
        soundTest1();
    }
    else if (!strcmp(argv[0], "soundTest2"))
    {
        soundTest2();
    }
    else if (!strcmp(argv[0], "soundTest3"))
    {
        soundTest3();
    }
    else
    {
        printf("Command: %s Failed to run!!!\n", argv[0]);
    }
}


