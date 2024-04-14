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
#include "ranging.h"
#include "controller.h"
#include "sound.h"

Controller controller;
VL53L4CD    vl53l4cd;

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
Function Name:  main
Input Args:     none
Output Args:    none
Description:    main function/application entry
/**********************************************/
int main(int argc, char *argv[])
{
	parseCommandLine(argc, argv);

    sleep(1);

    GpioController gpiocontroller;
    gpiocontroller.gpioInitializeLib();

    // initialize Controller
    Controller controllerInstance;
    controller = controllerInstance;


    // initialize Distance Sensors
    VL53L4CD vl53l4cdInstance;
    vl53l4cd = vl53l4cdInstance;
    vl53l4cd.rangingInit();

    initInterrupts();

    // initialize LEDS
    initLeds();

    // Initialize threads ////
    btbThread btbThread1;
    btbThread1.start();

    //// Initialize Timers ////
	btbTimer1 btbTimer_2p5ms;
	btbTimer_2p5ms.startns(BTB_TIMER_1_INTERVAL_NS);
    while(1)
    {
        //controller.primaryStateMachine();
    }

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
            ledSM();
            enableLedSM = false;
        }

        // controller Statemachine
        controller.primaryStateMachine();
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
    //printf("LEDTimerRunningCB\n");
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

    sensorValues senseValues;
    switch(gpio)
    {
        case D1_GPIO1:
            senseValues = vl53l4cd.rangingGetData(sensorID::SENSOR1);
            controller.updateSensorValue(senseValues, sensorID::SENSOR1);
            break;
        case D2_GPIO1:
            senseValues = vl53l4cd.rangingGetData(sensorID::SENSOR2);
            controller.updateSensorValue(senseValues, sensorID::SENSOR2);
            break;
        case D3_GPIO1:
            senseValues = vl53l4cd.rangingGetData(sensorID::SENSOR3);
            controller.updateSensorValue(senseValues, sensorID::SENSOR3);
            break;
        case D4_GPIO1:
            senseValues = vl53l4cd.rangingGetData(sensorID::SENSOR4);
            controller.updateSensorValue(senseValues, sensorID::SENSOR4);
            break;
        case D5_GPIO1:
            senseValues = vl53l4cd.rangingGetData(sensorID::SENSOR5);
            controller.updateSensorValue(senseValues, sensorID::SENSOR5);
            break;
        case D6_GPIO1:
            senseValues = vl53l4cd.rangingGetData(sensorID::SENSOR6);
            controller.updateSensorValue(senseValues, sensorID::SENSOR6);
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
        char * arr = new char[18]();
        ledCreateColorArr(arr, LED_COLOR_CYAN_DIM, LED_COLOR_PURPLE_DIM, LED_COLOR_YELLOW_DIM, LED_COLOR_WHITE_DIM, LED_COLOR_BLUE_DIM, LED_COLOR_GREEN_DIM);
        gpiocontroller.gpioLedSpiTest(arr);
    }
    else if (!strcmp(argv[0], "ledInitialiseTest"))
    {
        //ledInitialiseTest();
    }
    else if (!strcmp(argv[0], "interruptTest"))
    {
        GpioController gpiocontroller;
        gpiocontroller.gpioInitializeLib();
        Controller controllerInstance;
        controller = controllerInstance;

        // initialize Distance Sensors
        VL53L4CD vl53l4cdInstance;
        vl53l4cd = vl53l4cdInstance;
        vl53l4cd.rangingInit();

        // Initialize threads ////
        initInterrupts();

        btbThread btbThread1;
        btbThread1.start();
        controller.updateState(controllerState::TEST_DISTANCE_SENSORS);

        sleep(20);
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
        VL53L4CD vl54l4cd;
        vl54l4cd.rangingTestDistanceSensors();
    }
    else
    {
        printf("Command: %s Failed to run!!!\n", argv[0]);
    }
}

