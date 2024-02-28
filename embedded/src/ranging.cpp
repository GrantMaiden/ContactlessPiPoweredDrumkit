/****************************** Module Header ******************************\
Module Name:    ranging.cpp
Project:        btb
Author:         Lucas Zehner
Description:    ranging control for distance sensors
\***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"
#include "gpio_controller.h"
#include "ranging.h"

// VL53l4CD ULD Includes
//extern "C" {
//#include "VL53L4CD_api.h"
//#include "platform.h"
//#include "types.h"
//}

VL53L4CD_LinuxDev linuxDev1;
Dev_t i2cdev_Sensor1 = &linuxDev1;
VL53L4CD_LinuxDev linuxDev2;
Dev_t i2cdev_Sensor2 = &linuxDev2;
VL53L4CD_LinuxDev linuxDev3;
Dev_t i2cdev_Sensor3 = &linuxDev3;
VL53L4CD_LinuxDev linuxDev4;
Dev_t i2cdev_Sensor4 = &linuxDev4;
VL53L4CD_LinuxDev linuxDev5;
Dev_t i2cdev_Sensor5 = &linuxDev5;
VL53L4CD_LinuxDev linuxDev6;
Dev_t i2cdev_Sensor6 = &linuxDev6;

/**********************************************\
Function Name:  rangingTestDistanceSensors
Input Args:     none
Output Args:    none
Description:    tests distance sensors
/**********************************************/
void rangingTestDistanceSensors()
{
    printf("Beginning ranging test.\n");
    gpioInitializeLib();
    rangingInitDistanceSensors(SENSOR2, i2cdev_Sensor2);
    rangingInitDistanceSensors(SENSOR3, i2cdev_Sensor3);
    rangingInitDistanceSensors(SENSOR4, i2cdev_Sensor4);
    rangingInitDistanceSensors(SENSOR5, i2cdev_Sensor5);

}

/**********************************************\
Function Name:  rangingInitDistanceSensors
Input Args:     int xshutID: whichever sensor to init
Output Args:    none
Description:    intializes distance sensors
/**********************************************/
void rangingInitDistanceSensors(int xshutID, Dev_t dev)
{
    // Variables
	uint8_t status, loop, isReady;
	VL53L4CD_ResultsData_t 	results;
	//VL53L4CD_LinuxDev linuxDev;
	//Dev_t dev = &linuxDev;

	//enum xshutID ID = SENSOR1;

	// Selecting specific sensor with xshut
    if (!rangingSetXshut(xshutID))
    {
        printf("rangingSetXshut failed: %d\n", xshutID + 1);
        exit(0);
    }

    // Setting up comms
    status = VL53L4CD_comms_init(dev);
	if(status)
		printf("VL53L4CD comms init failed.\n");

    // Changing/confirming sensor addresses
    rangingChangeAddress(dev, xshutID);

    // Initialise sensor
    status = VL53L4CD_SensorInit(dev);
	if(status)
	{
		printf("VL53L4CD ULD Loading failed\n");
		exit(0);
    }
    printf("VL53L4CD ULD ready !\n");

    //Setting range timing to max speed (10ms)
    status = VL53L4CD_SetRangeTiming(dev, RANGE_TIMING_MAX, 0);
	if(status)
	{
		printf("VL53L4CD_SetRangeTiming failed with status %u\n", status);
		exit(0);
	}
	printf("Range timing set to %d ms.\n", RANGE_TIMING_MAX);

	//Setting detection thresholds
	status = VL53L4CD_SetDetectionThresholds(dev, RANGING_LIM_LOW, RANGING_LIM_HIGH, 3);
	if(status)
	{
		printf("VL53L4CD_SetDetectionThresholds failed with status %u\n", status);
		exit(0);
	}
	printf("Range window set to between %d mm and %d mm.\n", RANGING_LIM_LOW, RANGING_LIM_HIGH);

	//Start sensor ranging
	status = VL53L4CD_StartRanging(dev);
	if(status)
    {
		printf("VL53L4CD_StartRanging failed with status %u\n", status);
		exit(0);
	}
	printf("Ranging has started !\n");

	//Independent sensor ranging
    loop = 0;
	while(loop < 20)
	{
		/* Wait for hardware interrupt raised on GPIO1 pin */
		//isReady = WaitForL4CDInterrupt(dev);

		//if(isReady)
		//{
			/* (Mandatory) Clear HW interrupt to restart measurements */
			VL53L4CD_ClearInterrupt(dev);

			/* Read measured distance. RangeStatus = 0 means valid data */
			VL53L4CD_GetResult(dev, &results);
			printf("Status = %6u, Distance = %6u, Signal = %6u\n",
				 results.range_status,
				 results.distance_mm,
				 results.signal_per_spad_kcps);
			loop++;
		//}

		/* Wait a few ms to avoid too high polling (function in platform
		 * file, not in API) */
		WaitMs(dev, 5);
	}

	status = VL53L4CD_StopRanging(dev);
	printf("End of sensor %d ranging test !\n", xshutID + 1);
	printf("====================================================");
}


/**********************************************\
Function Name:  rangingSetXshut
Input Args:     int xshutID: Which sensor to enable
Output Args:    bool True if successfully switched Xshut low, False if failed
Description:    toggles distance sensors by setting Xshut pins
/**********************************************/

bool rangingSetXshut(int xshutID)
{
    char pins = 0;
    int status = 0;
    pins = 1 << xshutID;
    if (xshutID == SENSORALL)
        pins = 0xff;
    status = status | gpioSetOutput(D1_XSHUT, pins & 0b1);
    status = status | gpioSetOutput(D2_XSHUT, (pins & 0b10) >> 1);
    status = status | gpioSetOutput(D3_XSHUT, (pins & 0b100) >> 2);
    status = status | gpioSetOutput(D4_XSHUT, (pins & 0b1000) >> 3);
    status = status | gpioSetOutput(D5_XSHUT, (pins & 0b10000) >> 4);
    status = status | gpioSetOutput(D6_XSHUT, (pins & 0b100000) >> 5);

    //gpioSetOutput(D1_XSHUT, PI_HIGH);
    //gpioSetOutput(D2_XSHUT, PI_LOW);
    //gpioSetOutput(D3_XSHUT, PI_LOW);
    //gpioSetOutput(D4_XSHUT, PI_LOW);
    //gpioSetOutput(D5_XSHUT, PI_LOW);
    //gpioSetOutput(D6_XSHUT, PI_LOW);

    if (status)
        return false;
    else
        return true;
}

/**********************************************\
Function Name:  rangingChangeAddress
Input Args:     Dev_t dev
                int xshutID
Output Args:    none
Description:    changes received sensor address
/**********************************************/
void rangingChangeAddress(Dev_t dev, int xshutID)
{
    VL53L4CD_LinuxDev linuxDev;
    Dev_t devExpected = &linuxDev;
    devExpected->fd = dev->fd;

    dev->address = I2C_ADDRESS_INIT;
    devExpected->address = I2C_ADDRESS_INIT + xshutID;
    VL53L4CD_Error status = VL53L4CD_SetI2CAddress(dev, devExpected->address);
    if (status == VL53L4CD_ERROR_NONE)
    {
        printf("Successfully changed address of sensor %d from default.\n", xshutID + 1);
        dev->address = devExpected->address;
        return;
    }
    else if (VL53L4CD_SetI2CAddress(devExpected, devExpected->address) == VL53L4CD_ERROR_NONE)
    {
        printf("Sensor %d address already successfully changed.\n", xshutID + 1);
        dev->address = devExpected->address;
        return;
    }
    else
    {
        printf("Error: Sensor %d address lost, status: %u, exiting...\n", xshutID + 1, status);
        printf("Dev address is %d.\n",dev->address);
        printf("DevExpected address is %d.\n",devExpected->address);
        exit(0);
    }
}





















