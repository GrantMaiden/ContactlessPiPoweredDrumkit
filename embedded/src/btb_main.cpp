/****************************** Module Header ******************************\
Module Name:    btb_main.cpp
Project:        btb
Author:         Grant Maiden
Description:    Entry Point into ENG5228 project for University of Glasgow
\***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "btb_main.h"

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
int main()
{
	//Initialize Application
    initDistanceSensors();
    initLeds();
    initInterrupts();

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
