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
    rangingInitDistanceSensors();

}

/**********************************************\
Function Name:  rangingInitDistanceSensors
Input Args:     none
Output Args:    none
Description:    intializes distance sensors
/**********************************************/
void rangingInitDistanceSensors()
{
    // Variables
	int status;
	VL53L4CD_LinuxDev linuxDev;
	Dev_t dev = &linuxDev;

	//enum xshutID ID = SENSOR1;

	// Power on sensor and init

    if (!rangingSetXshut(SENSOR2))
    {
        printf("rangingSetXshut failed: %d", SENSOR2);
        exit(0);
    }
    status = VL53L4CD_comms_init(dev);
	if(status)
		printf("VL53L4CD comms init failed\n");

    rangingChangeAddress(dev, SENSOR2);

//    rangingSetXshut(sensor2);
//    rangingSetXshut(sensor3);
//    rangingSetXshut(sensor4);
//    rangingSetXshut(sensor5);
//    rangingSetXshut(sensor6);
//    rangingSetXshut(sensorAll);

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

    Dev_t devExpected = dev;
    dev->address = I2C_ADDRESS_INIT;
    devExpected->address = I2C_ADDRESS_INIT + xshutID;
    VL53L4CD_Error status = VL53L4CD_SetI2CAddress(dev, devExpected->address);
    if (status == VL53L4CD_ERROR_NONE)
    {
        printf("Successfully changed address of sensor %d from default", xshutID + 1);
                VL53L4CD_StartRanging(devExpected);
        return;
    }
    else if (VL53L4CD_SetI2CAddress(devExpected, devExpected->address) == VL53L4CD_ERROR_NONE)
    {
        printf("Sensor %d address already successfully changed", xshutID + 1);
        return;
    }
    else
    {
        printf("Error: Sensor %d address lost, status: %u, exiting...", xshutID + 1, status);
        exit(0);
    }
}





















