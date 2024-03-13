/****************************** Module Header ******************************\
Module Name:    ranging.h
Project:        btb
Author:         Lucas Zehner
Description:    contains ranging definitions and headers
\***************************************************************************/
#ifndef RANGING_H
#define RANGING_H
class GpioController;


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

using namespace std;
class VL53L4CD
{
    public:
        //Functions

        /**
         * Initialize disatance sensor globals
         **/
        void rangingInit();

        /**
         * Test Distance Sensors
         **/
        void rangingTestDistanceSensors();

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

    private:
        // private global variables
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


        /**
         * Initialize Distance Sensors
         * \param int id whichever sensor to init
         **/
        void rangingInitDistanceSensor(int id, Dev_t dev);

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
         * Polls all sensors simultaneously
         **/
        void rangingPollingTestAll();


};


#endif // RANGING_H










