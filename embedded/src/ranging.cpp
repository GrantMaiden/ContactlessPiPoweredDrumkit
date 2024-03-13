/****************************** Module Header ******************************\
Module Name:    ranging.cpp
Project:        btb
Author:         Lucas Zehner & Ethan Travers
Description:    ranging control for distance sensors
\***************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "defines.h"
#include "gpio_controller.h"
#include "ranging.h"


/**********************************************\
Function Name:  rangingInit
Input Args:     none
Output Args:    none
Description:    Initialize disatance sensor globals
/**********************************************/
void VL53L4CD::rangingInit()
{
    GpioController gpioController;
    gpioController.gpioInitializeLib();

    i2cdev_Sensor1->address = I2C_ADDRESS_INIT;
    i2cdev_Sensor2->address = I2C_ADDRESS_INIT;
    i2cdev_Sensor3->address = I2C_ADDRESS_INIT;
    i2cdev_Sensor4->address = I2C_ADDRESS_INIT;
    i2cdev_Sensor5->address = I2C_ADDRESS_INIT;
    i2cdev_Sensor6->address = I2C_ADDRESS_INIT;

    gpioController.gpioSetOutput(D1_XSHUT, PI_LOW);
    gpioController.gpioSetOutput(D2_XSHUT, PI_LOW);
    gpioController.gpioSetOutput(D3_XSHUT, PI_LOW);
    gpioController.gpioSetOutput(D4_XSHUT, PI_LOW);
    gpioController.gpioSetOutput(D5_XSHUT, PI_LOW);
    gpioController.gpioSetOutput(D6_XSHUT, PI_LOW);

    rangingInitDistanceSensor(SENSOR1, i2cdev_Sensor1);
    rangingInitDistanceSensor(SENSOR2, i2cdev_Sensor2);
    rangingInitDistanceSensor(SENSOR3, i2cdev_Sensor3);
    rangingInitDistanceSensor(SENSOR4, i2cdev_Sensor4);
    rangingInitDistanceSensor(SENSOR5, i2cdev_Sensor5);
    rangingInitDistanceSensor(SENSOR6, i2cdev_Sensor6);

    VL53L4CD_StartRanging(i2cdev_Sensor1);
    //usleep(500);
    VL53L4CD_StartRanging(i2cdev_Sensor2);
    //usleep(500);
    VL53L4CD_StartRanging(i2cdev_Sensor3);
    //usleep(500);
    VL53L4CD_StartRanging(i2cdev_Sensor4);
    //usleep(500);
    VL53L4CD_StartRanging(i2cdev_Sensor5);
    //usleep(500);
    VL53L4CD_StartRanging(i2cdev_Sensor6);

}


/**********************************************\
Function Name:  rangingTestDistanceSensors
Input Args:     none
Output Args:    none
Description:    tests distance sensors
/**********************************************/
void VL53L4CD::rangingTestDistanceSensors()
{
    printf("Beginning ranging test.\n");
    rangingInit();

    rangingPollingTestAll();
}


/**********************************************\
Function Name:  rangingInitDistanceSensors
Input Args:     int id: whichever sensor to init
Output Args:    none
Description:    intializes distance sensors
/**********************************************/
void VL53L4CD::rangingInitDistanceSensor(int id, Dev_t dev)
{
    // Variables
	uint8_t status, loop, isReady;

	// Selecting specific sensor with xshut

    if (!rangingSetXshut(id))
    {
        printf("rangingSetXshut failed: %d\n", id + 1);
        exit(0);
    }

    // Setting up comms
    status = VL53L4CD_comms_init(dev);
	if(status > 0)
		printf("VL53L4CD comms init failed.\n");

	// Changing/confirming sensor addresses
    //

    // Initialise sensor
    status = VL53L4CD_SensorInit(dev);
	if(status > 0)
	{
		printf("VL53L4CD ULD Loading failed, trying pre-programmed address, Status: %i\n", status);
		dev->address = dev->address +16*id + 16;
		status = VL53L4CD_SensorInit(dev);
		if(status)
		{
            printf("VL53L4CD Can't find correct address, Status: %i\n", status);
            exit(0);
		}

    }
    printf("VL53L4CD ULD ready !\n");

    rangingChangeAddress(dev, id);

    //Setting range timing to max speed (10ms)
    status = VL53L4CD_SetRangeTiming(dev, RANGE_TIMING_MAX, 0);
	if(status)
	{
		printf("VL53L4CD_SetRangeTiming failed with status %u\n", status);
		exit(0);
	}
	printf("Range timing set to %d ms.\n", RANGE_TIMING_MAX);

	//printf("Signal Threshold set as: %u\n", VL53L4CD_SetSignalThreshold(dev, 200));
	//Setting detection thresholds
	//status = VL53L4CD_SetDetectionThresholds(dev, RANGING_LIM_LOW, RANGING_LIM_HIGH, 3);
	if(status)
	{
		printf("VL53L4CD_SetDetectionThresholds failed with status %u\n", status);
		exit(0);
	}
	printf("Range window set to between %d mm and %d mm.\n", RANGING_LIM_LOW, RANGING_LIM_HIGH);


	printf("End of sensor %d init !\n", id + 1);
	printf("====================================================\n");

}


/**********************************************\
Function Name:  rangingSetXshut
Input Args:     int id: Which sensor to enable
Output Args:    bool True if successfully switched Xshut low, False if failed
Description:    toggles distance sensors by setting Xshut pins
/**********************************************/

bool VL53L4CD::rangingSetXshut(int id)
{
    GpioController gpioController;

    switch(id)
    {
        case SENSOR1:
            gpioController.gpioSetOutput(D1_XSHUT, PI_HIGH);
            break;
        case SENSOR2:
            gpioController.gpioSetOutput(D2_XSHUT, PI_HIGH);
            break;
        case SENSOR3:
            gpioController.gpioSetOutput(D3_XSHUT, PI_HIGH);
            break;
        case SENSOR4:
            gpioController.gpioSetOutput(D4_XSHUT, PI_HIGH);
            break;
        case SENSOR5:
            gpioController.gpioSetOutput(D5_XSHUT, PI_HIGH);
            break;
        case SENSOR6:
            gpioController.gpioSetOutput(D6_XSHUT, PI_HIGH);
            break;
        default:
            break;
    }

    usleep(5000);

    //if (status)
   //    return false;
    //else
        return true;
}

/**********************************************\
Function Name:  rangingChangeAddress
Input Args:     Dev_t dev
                int id
Output Args:    none
Description:    changes received sensor address
/**********************************************/
void VL53L4CD::rangingChangeAddress(Dev_t dev, int id)
{
    VL53L4CD_LinuxDev linuxDev;
    Dev_t devExpected = &linuxDev;
    devExpected->fd = dev->fd;

    dev->address = I2C_ADDRESS_INIT;
    devExpected->address = I2C_ADDRESS_INIT + 16*id + 16;
    VL53L4CD_Error status = VL53L4CD_SetI2CAddress(dev, devExpected->address);
    if (status == VL53L4CD_ERROR_NONE)
    {
        printf("Successfully changed address of sensor %d from default to %u.\n", id + 1,devExpected->address);
        dev->address = devExpected->address;
        return;
    }
    else if (VL53L4CD_SetI2CAddress(devExpected, devExpected->address) == VL53L4CD_ERROR_NONE)
    {
        printf("Sensor %d address already successfully changed.\n", id + 1);
        dev->address = devExpected->address;
        return;
    }
    else
    {
        printf("Error: Sensor %d address lost, status: %u, exiting...\n", id + 1, status);
        printf("Dev address is %d.\n",dev->address);
        printf("DevExpected address is %d.\n",devExpected->address);
        exit(0);
    }
}

/**********************************************\
Function Name:  rangingGetData
Input Args:     Dev_t dev- i2c device
Output Args:    sensorValues - sensorValues object
Description:    Get data from input dev id
/**********************************************/
sensorValues VL53L4CD::rangingGetData(sensorID sensor)
{
    VL53L4CD_LinuxDev linuxDev1;
    Dev_t devTemp = &linuxDev1;
    switch (sensor)
    {
        case sensorID::SENSOR1:
            devTemp = i2cdev_Sensor1;
            break;
        case sensorID::SENSOR2:
            devTemp = i2cdev_Sensor2;
            break;
        case sensorID::SENSOR3:
            devTemp = i2cdev_Sensor3;
            break;
        case sensorID::SENSOR4:
            devTemp = i2cdev_Sensor4;
            break;
        case sensorID::SENSOR5:
            devTemp = i2cdev_Sensor5;
            break;
        case sensorID::SENSOR6:
            devTemp = i2cdev_Sensor6;
            break;
        default:
            break;
    }
    VL53L4CD_ResultsData_t temp;
    sensorValues sensorVals;

    VL53L4CD_ClearInterrupt(devTemp);
    VL53L4CD_GetResult(devTemp, &temp);
    sensorVals.rangeStatus = temp.range_status;
    sensorVals.currentDistance_mm = temp.distance_mm;

    return sensorVals;

}

/**********************************************\
Function Name:  rangingCheckIfReady
Input Args:     sensor - sensorID
Output Args:    bool
Description:    returns if a snesor has data ready
/**********************************************/
bool VL53L4CD::rangingCheckIfReady(sensorID sensor)
{
    VL53L4CD_LinuxDev linuxDev1;
    Dev_t devTemp = &linuxDev1;
    switch (sensor)
    {
        case sensorID::SENSOR1:
            devTemp = i2cdev_Sensor1;
            break;
        case sensorID::SENSOR2:
            devTemp = i2cdev_Sensor2;
            break;
        case sensorID::SENSOR3:
            devTemp = i2cdev_Sensor3;
            break;
        case sensorID::SENSOR4:
            devTemp = i2cdev_Sensor4;
            break;
        case sensorID::SENSOR5:
            devTemp = i2cdev_Sensor5;
            break;
        case sensorID::SENSOR6:
            devTemp = i2cdev_Sensor6;
            break;
        default:
            break;
    }
    uint8_t isReady;
    VL53L4CD_CheckForDataReady(i2cdev_Sensor1, &isReady);

    if (isReady != 1)
        return true;
    else
        return false;

}

/**********************************************\
Function Name:  rangingPollingTestAll
Input Args:     none
Output Args:    none
Description:    polls all sensors simultaneously
                (not working)
/**********************************************/
void VL53L4CD::rangingPollingTestAll()
{
	int loop = 0;
	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	int status5 = 0;
	int status6 = 0;

    uint8_t isReady1 = 0;
	uint8_t isReady2 = 0;
	uint8_t isReady3 = 0;
	uint8_t isReady4 = 0;
	uint8_t isReady5 = 0;
	uint8_t isReady6 = 0;

    printf("Dev address is: %i\n", i2cdev_Sensor1->address);
    status1 = VL53L4CD_StartRanging(i2cdev_Sensor1);
    if(status1)
    {
		printf("VL53L4CD_StartRanging failed on sensor %d with status %u\n", 1, status1);
		exit(0);
	}
    printf("Ranging started for sensor %d !\n", 1);
    VL53L4CD_ResultsData_t temp;
	while(loop < 400)
	{
        VL53L4CD_CheckForDataReady(i2cdev_Sensor1, &isReady1);
		if(isReady1)
		{
            VL53L4CD_ClearInterrupt(i2cdev_Sensor1);
            VL53L4CD_GetResult(i2cdev_Sensor1, &temp);

            printf("S1 = %u\tS2 = %u\tS3 = %u\tS4 = %u\tS5 = %u\tS6 = %u\n",
				 temp.distance_mm,
				 0,
				 0,
				 0,
				 0,
				 0);
            loop++;
        }

		/* Wait a few ms to avoid too high polling (function in platform
		 * file, not in API) */
		WaitMs(i2cdev_Sensor1, 5);
	}

	VL53L4CD_StopRanging(i2cdev_Sensor1);
}




















