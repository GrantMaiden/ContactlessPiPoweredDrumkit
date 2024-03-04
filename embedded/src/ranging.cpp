/****************************** Module Header ******************************\
Module Name:    ranging.cpp
Project:        btb
Author:         Lucas Zehner & Ethan Travers
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

VL53L4CD_ResultsData_t 	results_Sensor1;
VL53L4CD_ResultsData_t 	results_Sensor2;
VL53L4CD_ResultsData_t 	results_Sensor3;
VL53L4CD_ResultsData_t 	results_Sensor4;
VL53L4CD_ResultsData_t 	results_Sensor5;
VL53L4CD_ResultsData_t 	results_Sensor6;

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
    //rangingPollingTestAll();
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

	printf("End of sensor %d init !\n", xshutID + 1);
	printf("====================================================\n");
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

/**********************************************\
Function Name:  rangingInterruptPoll
Input Args:     Dev_t dev- i2c device
                int sensorID- which sensor to store
                data for
Output Args:    none
Description:    changes received sensor address
/**********************************************/
void rangingInterruptPoll(Dev_t dev, int sensorID)
{
	VL53L4CD_ClearInterrupt(dev);

	switch(sensorID)
	case 1:
        VL53L4CD_GetResult(dev, &results_Sensor1);
        break;
    case 2:
        VL53L4CD_GetResult(dev, &results_Sensor2);
        break;
    case 3:
        VL53L4CD_GetResult(dev, &results_Sensor3);
        break;
    case 4:
        VL53L4CD_GetResult(dev, &results_Sensor4);
        break;
    case 5:
        VL53L4CD_GetResult(dev, &results_Sensor5);
        break;
    case 6:
        VL53L4CD_GetResult(dev, &results_Sensor6);
        break;
    default:
        break;

}

/**********************************************\
Function Name:  rangingPollingTestAll
Input Args:     none
Output Args:    none
Description:    polls all sensors simultaneously
                (not working)
/**********************************************/
void rangingPollingTestAll()
{
	int loop = 0;
//	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	int status5 = 0;
	uint8_t isReady2 = 0;
	uint8_t isReady3 = 0;
	uint8_t isReady4 = 0;
	uint8_t isReady5 = 0;
//	int status6 = 0;

    //status1 = VL53L4CD_StartRanging(i2cdev_Sensor1);




	//status6 = VL53L4CD_StartRanging(i2cdev_Sensor6);

//        if(status1)
//    {
//		printf("VL53L4CD_StartRanging failed on sensor %d with status %u\n", 1, status1);
//		exit(0);
//	  }
//		if(status2)
//    {
//		printf("VL53L4CD_StartRanging failed on sensor %d with status %u\n", 2, status2);
//		exit(0);
//	}
//		if(status3)
//    {
//		printf("VL53L4CD_StartRanging failed on sensor %d with status %u\n", 3, status3);
//		exit(0);
//	}
//		if(status4)
//    {
//		printf("VL53L4CD_StartRanging failed on sensor %d with status %u\n", 4, status4);
//		exit(0);
//	}
//		if(status5)
//    {
//		printf("VL53L4CD_StartRanging failed on sensor %d with status %u\n", 5, status5);
//		exit(0);
//	}
//        if(status6)
//    {
//		printf("VL53L4CD_StartRanging failed on sensor %d with status %u\n", 6, status6);
//		exit(0);
//	}

	while(loop < 2000)
	{
		status2 = VL53L4CD_StartRanging(i2cdev_Sensor2);
		VL53L4CD_CheckForDataReady(i2cdev_Sensor2, &isReady2);
		if(isReady2)
		{
            VL53L4CD_ClearInterrupt(i2cdev_Sensor2);
            VL53L4CD_GetResult(i2cdev_Sensor2, &results_Sensor2);
        }
        VL53L4CD_StopRanging(i2cdev_Sensor2);
        printf("S2 = %u\t",results_Sensor2.distance_mm);

        status3 = VL53L4CD_StartRanging(i2cdev_Sensor3);
        VL53L4CD_CheckForDataReady(i2cdev_Sensor3, &isReady3);
		if(isReady3)
		{
            VL53L4CD_ClearInterrupt(i2cdev_Sensor3);
            VL53L4CD_GetResult(i2cdev_Sensor3, &results_Sensor3);
        }
        VL53L4CD_StopRanging(i2cdev_Sensor3);
        printf("S3 = %u\t",results_Sensor3.distance_mm);

        status4 = VL53L4CD_StartRanging(i2cdev_Sensor4);
        VL53L4CD_CheckForDataReady(i2cdev_Sensor4, &isReady4);
       	if(isReady4)
		{
            VL53L4CD_ClearInterrupt(i2cdev_Sensor4);
            VL53L4CD_GetResult(i2cdev_Sensor4, &results_Sensor4);
        }
        VL53L4CD_StopRanging(i2cdev_Sensor4);
        printf("S4 = %u\t",results_Sensor4.distance_mm);

        status5 = VL53L4CD_StartRanging(i2cdev_Sensor5);
        VL53L4CD_CheckForDataReady(i2cdev_Sensor5, &isReady5);
		if(isReady5)
		{
            VL53L4CD_ClearInterrupt(i2cdev_Sensor5);
            VL53L4CD_GetResult(i2cdev_Sensor5, &results_Sensor5);
        }
        VL53L4CD_StopRanging(i2cdev_Sensor5);
        printf("S5 = %u\n",results_Sensor5.distance_mm);

//			printf("S1 = %u\tS2 = %u\tS3 = %u\tS4 = %u\tS5 = %u\tS6 = %u\n",
//				 results_Sensor1.distance_mm,
//				 results_Sensor2.distance_mm,
//				 results_Sensor3.distance_mm,
//				 results_Sensor4.distance_mm,
//				 results_Sensor5.distance_mm,
//				 results_Sensor6.distance_mm);
			loop++;

		/* Wait a few ms to avoid too high polling (function in platform
		 * file, not in API) */
		//WaitMs(dev, 5);
	}

	//VL53L4CD_StopRanging(i2cdev_Sensor1);




	//VL53L4CD_StopRanging(i2cdev_Sensor6);
}




















