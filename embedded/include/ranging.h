/****************************** Module Header ******************************\
Module Name:    ranging.h
Project:        btb
Author:         Lucas Zehner
Description:    contains ranging definitions and headers
\***************************************************************************/

// Includes
extern "C" {
#include "VL53L4CD_api.h"
#include "platform.h"
#include "types.h"
}

// Defines
#define I2C_ADDRESS_INIT    0x52
#define D1_I2C_ADDRESS      0x52
#define D2_I2C_ADDRESS      0x54
#define D3_I2C_ADDRESS      0x56
#define D4_I2C_ADDRESS      0x58
#define D5_I2C_ADDRESS      0x60
#define D6_I2C_ADDRESS      0x62
// Change these as desired
#define RANGE_TIMING_MAX    10
#define RANGING_LIM_LOW     10
#define RANGING_LIM_HIGH    500


// Enums
enum xshutID
{
    SENSOR1 = 0,
    SENSOR2 = 1,
    SENSOR3 = 2,
    SENSOR4 = 3,
    SENSOR5 = 4,
    SENSOR6 = 5,
    SENSORALL = 6

};

// Structs


//Functions
/**
 * Initialize disatance sensor globals
 **/
void rangingInit();

/**
 * Runs range test from selected sensor
 * \param dev Dev_t objec that contains sensor identifier and i2c address
 * \returns status enum of sensor
 **/
int getRangeTest(Dev_t dev);

/**
 * Initialize Distance Sensors
 * \param int id whichever sensor to init
 **/
void rangingInitDistanceSensors(int id, Dev_t dev);

/**
 * Test Distance Sensors
 **/
void rangingTestDistanceSensors();

/**
 * Toggles Between Sensors
 * \param id- enum which sensor to enable
 * \returns bool- True if successfully switched Xshut low, False if failed
 **/
bool rangingSetXshut(int id);

/**
 * Changes received sensor address
 * \param dev- Dev_t dev device object
 * \param id- int which sensor to enable
 **/
void rangingChangeAddress(Dev_t dev, int id);

/**
 * Gets sensor data after interrupt
 * \param dev- Dev_t dev device object
 * \param sensoriD- int which sensor is data stored for
 **/
void rangingInterruptPoll(Dev_t dev);

/**
 * Polls all sensors simultaneously
 **/
void rangingPollingTestAll();

