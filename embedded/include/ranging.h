/****************************** Module Header ******************************\
Module Name:    ranging.h
Project:        btb
Author:         Lucas Zehner
Description:    contains ranging definitions and headers
\***************************************************************************/

#define VL53L4CD_I2C_FAST_MODE_PLUS
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
 * Get data from input dev id
 * \param sensor - sensorID
 * \returns sensorValues
 **/
sensorValues rangingGetData(sensorID sensor);

/**
 * returns if a snesor has data ready
 * \param sensor - sensorID
 * \returns bool
 **/
bool rangingCheckIfReady(sensorID sensor);

/**
 * Polls all sensors simultaneously
 **/
void rangingPollingTestAll();

