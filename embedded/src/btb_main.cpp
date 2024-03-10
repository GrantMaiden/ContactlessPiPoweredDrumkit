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
	//// Parse Unit Tests and Other Command Line Args ////
	parseCommandLine(argc, argv);

    sleep(1);
    gpioInitializeLib();

    // initialize Distance Sensors
    rangingInit();

    // Initialize threads ////
    initInterrupts();
    //intiLeds();

    btbThread btbThread1;
    btbThread1.start();

    //// Initialize Timers ////
	//btbTimer1 btbTimer_2p5ms;
	//btbTimer_2p5ms.startns(BTB_TIMER_1_INTERVAL_NS);
    while(1)
    {
    }

}

/**********************************************\
Function Name:  initLeds()
Input Args:     none
Output Args:    none
Description:    intialize Leds
/**********************************************/
void initLeds()
{
    char * colorArr = new char[18]();
    ledStateMachine currentState = ledStateMachine::INITIAL1;
    unsigned initialColour1 = 0x7;
    ledStateMachine nextState;
    int loops = 0;
    int loopWait = 0;
    int initialSpeed = INITIAL_LOOP_WAIT;
    char colorByte = 0x00;
    unsigned ColorRed = colorByte << 16;
    unsigned ColorGreen = colorByte << 8;
    unsigned ColorBlue = colorByte << 0;
    unsigned ColorYellow = ColorRed + ColorGreen;
    unsigned ColorCyan = ColorGreen + ColorBlue;
    unsigned ColorPurple = ColorRed + ColorBlue;
    unsigned ColourWhite = ColorRed + ColorGreen + ColorBlue;
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
            ledSM(); // Calls led state machine
            enableLedSM = false;
        }

        // controller Statemachine
        controllerSM();
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
        //printf("GPIO %d returned Interrupt Timeout! Interrupt Exceeded %dms!\nUs tick: %lu\n", gpio, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, tick);
        ;
    }

    //printf("GPIO %i\n", gpio);

//    switch(gpio)
//    {
//        case D1_GPIO1:
//            sens1DataReady = 1;
//            break;
//        case D2_GPIO1:
//            sens2DataReady = 1;
//            break;
//        case D3_GPIO1:
//            sens3DataReady = 1;
//            break;
//        case D4_GPIO1:
//            sens4DataReady = 1;
//            break;
//        case D5_GPIO1:
//            sens5DataReady = 1;
//            break;
//        case D6_GPIO1:
//            sens6DataReady = 1;
//            break;
//    }

    sensorValues senseValues;
    switch(gpio)
    {
        case D1_GPIO1:
            senseValues = rangingGetData(sensorID::SENSOR1);
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR1);
            break;
        case D2_GPIO1:
            senseValues = rangingGetData(sensorID::SENSOR2);
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR2);
            break;
        case D3_GPIO1:
            senseValues = rangingGetData(sensorID::SENSOR3);
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR3);
            break;
        case D4_GPIO1:
            senseValues = rangingGetData(sensorID::SENSOR4);
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR4);
            break;
        case D5_GPIO1:
            senseValues = rangingGetData(sensorID::SENSOR5);
            controllerUpdateSensorValue(senseValues, sensorID::SENSOR5);
            break;
        case D6_GPIO1:
            senseValues = rangingGetData(sensorID::SENSOR6);
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
    else if (!strcmp(argv[0], "ledFadeTest"))
    {
        gpioInitializeLib();
        char colorByte = 0x00;
        char * colorArr = new char[18]();
        ledFadeTest(2, WAIT_5MS);
    }
    else if (!strcmp(argv[0], "ledFlash"))
    {
        gpioInitializeLib();
        ledFlashTest(LED_COLOR_RED, 3, 1, 3);
    }
    else if (!strcmp(argv[0], "interruptTest"))
    {
        gpioInitializeLib();

        // initialize Distance Sensors
        rangingInit();

        // Initialize threads ////
        initInterrupts();

        btbThread btbThread1;
        btbThread1.start();
        //usleep(3000);
        controllerUpdateState(controllerState::TEST_DISTANCE_SENSORS);

        sleep(20);
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
    else if (!strcmp(argv[0], "rangingTestDistanceSensors"))
    {
        rangingTestDistanceSensors();
    }
    else
    {
        printf("Command: %s Failed to run!!!\n", argv[0]);
    }
}

