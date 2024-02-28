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

#include "btb_main.h"
#include "gpio_controller.h"
#include "defines.h"
#include "led.h"

// VL53l4CD ULD Includes
extern "C" {
#include "VL53L4CD_api.h"
#include "platform.h"
#include "types.h"
}

/**********************************************\
Function Name:  main
Input Args:     none
Output Args:    none
Description:    main function/application entry
/**********************************************/
int main(int argc, char *argv[])
{
    parseCommandLine(argc, argv);
    //gpioTest(GPIO_TEST_LENGTH_SEC);
	//Initialize Application
	gpioInitializeLib();
    initLeds();
    //initInterrupts();
    //// Example: Start Ranging on D2
    gpioSetOutput(D1_XSHUT, PI_LOW);
    gpioSetOutput(D2_XSHUT, PI_LOW);
    gpioSetOutput(D3_XSHUT, PI_LOW);
    gpioSetOutput(D4_XSHUT, PI_HIGH);
    gpioSetOutput(D5_XSHUT, PI_LOW);
    gpioSetOutput(D6_XSHUT, PI_LOW);
    //sleep(1); //let gpio change
    initDistanceSensors();


}

/**********************************************\
Function Name:  initDistanceSensors
Input Args:     none
Output Args:    none
Description:    intializes distance sensors
/**********************************************/
void initDistanceSensors()
{
    // Variables
	int status;
	VL53L4CD_LinuxDev LinuxDev;
	Dev_t Dev = &LinuxDev;

	// Power on sensor and init
	status = VL53L4CD_comms_init(Dev);
	if(status)
	{
		printf("VL53L4CD comms init failed\n");
	}

	// Optionally Run example1 from sensor demo
    example1(Dev);
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
Function Name:  rangingISRCallback()
Input Args:     int gpio :Triggered Gpio number
                int level: GPIO input level at time of ISR
                uint32_t tick: trigger time in microseconds.
Output Args:    none
Description:    callback that is triggered on DistanceSensors Interrupt Falling Edge.
/**********************************************/
void rangingISRCallback(int gpio, int level, uint32_t tick)
{
    printf("GPIO %d became %d at %d\n", gpio, level, tick); //COMMENT ME OUT WHEN WORKING

    if (level == PI_TIMEOUT)
    {
        printf("GPIO %d returned Interrupt Timeout! Interrupt Exceeded %dms!\n", gpio, DISTANCE_SENSOR_INTERRUPT_TIMEOUT);
    }
    // LUCAS ETHAN ADD CODE.
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
    else if (!strcmp(argv[0], "interruptTest"))
    {
        //gpioLedSpiTest(arr);
    }
    else
    {
        printf("Command: %s Failed to run!!!\n", argv[0]);
    }
}

/**********************************************\
Function Name:  example1
Input Args:     Dev_t dev: Device struct for Distance Sensor
Output Args:    none
Description:    runs example1 from distance Sensor STM32 example code
/**********************************************/
int example1(Dev_t dev)
{

	/*********************************/
	/*   VL53L4CD ranging variables  */
	/*********************************/

	uint8_t 				status, loop, isReady;
	uint16_t 				sensor_id;
	VL53L4CD_ResultsData_t 			results;		/* results data from VL53L4CD */


	/*********************************/
	/*      Customer platform        */
	/*********************************/

	/* Default VL53L4CD I2C address */
	dev->address = 0x52;

	/* (Optional) Change I2C address */
	//status = VL53L4CD_SetI2CAddress(dev, 0x20);
	//dev->address = 0x20;


	/*********************************/
	/*   Power on sensor and init    */
	/*********************************/

	/* (Optional) Check if there is a VL53L4CD sensor connected */
	status = VL53L4CD_GetSensorId(dev, &sensor_id);
	if(status || (sensor_id != 0xEBAA))
	{
		printf("VL53L4CD not detected at requested address\n");
		return status;
	}

	/* (Mandatory) Init VL53L4CD sensor */
	status = VL53L4CD_SensorInit(dev);
	if(status)
	{
		printf("VL53L4CD ULD Loading failed\n");
		return status;
	}

	printf("VL53L4CD ULD ready !\n");

	/*********************************/
	/*         Ranging loop          */
	/*********************************/

	status = VL53L4CD_StartRanging(dev);

	loop = 0;
	while(loop < 200)
	{
		/* Use polling function to know when a new measurement is ready.
		 * Another way can be to wait for HW interrupt raised on PIN 7
		 * (GPIO 1) when a new measurement is ready */

		status = VL53L4CD_CheckForDataReady(dev, &isReady);

		if(isReady)
		{
			/* (Mandatory) Clear HW interrupt to restart measurements */
			VL53L4CD_ClearInterrupt(dev);

			/* Read measured distance. RangeStatus = 0 means valid data */
			VL53L4CD_GetResult(dev, &results);
			printf("Status = %6u, Distance = %6u, Signal = %6u\n",
				 results.range_status,
				 results.distance_mm,
				 results.signal_per_spad_kcps);
			loop++;
		}

		/* Wait a few ms to avoid too high polling (function in platform
		 * file, not in API) */
		WaitMs(dev, 5);
	}

	status = VL53L4CD_StopRanging(dev);
	printf("End of ULD demo\n");
	return status;
}
