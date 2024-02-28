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
#define D2_I2C_ADDRESS      0x53
#define D3_I2C_ADDRESS      0x54
#define D4_I2C_ADDRESS      0x55
#define D5_I2C_ADDRESS      0x56
#define D6_I2C_ADDRESS      0x57
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
 * Runs range test from selected sensor
 * \param dev Dev_t objec that contains sensor identifier and i2c address
 * \returns status enum of sensor
 **/
int getRangeTest(Dev_t dev);

/**
 * Initialize Distance Sensors
 * \param int xshutID whichever sensor to init
 **/
void rangingInitDistanceSensors(int xshutID, Dev_t dev);

/**
 * Test Distance Sensors
 **/
void rangingTestDistanceSensors();

/**
 * Toggles Between Sensors
 * \param xshutID- enum which sensor to enable
 * \returns bool- True if successfully switched Xshut low, False if failed
 **/
bool rangingSetXshut(int xshutID);

/**
 * changes received sensor address
 * \param dev- Dev_t dev device object
 * \param xshutID- int which sensor to enable
 **/
void rangingChangeAddress(Dev_t dev, int xshutID);


