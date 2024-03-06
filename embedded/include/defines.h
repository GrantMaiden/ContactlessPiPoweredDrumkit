/****************************** Module Header ******************************\
Module Name:    defines.h
Project:        btb
Author:         Grant Maiden
Description:    contains user constants, global enums, and structs
\***************************************************************************/

// Test defines
#define GPIO_TEST_LENGTH_SEC                10
#define TOTAL_INSTRUMENTS_SOUND_TEST2       3

// Misc
#define DISTANCE_SENSOR_INTERRUPT_TIMEOUT   12
#define BTB_TIMER_1_INTERVAL_NS             2.5*1000000

// Gpio Controller Defines
#define WAIT_10MS       1551000
#define WAIT_5MS        775000
#define WAIT_2MS        310200
#define WAIT_1MS        155100
#define WAIT_100US      15510
#define WAIT_245NS      38
#define WAIT_190NS      29
#define WAIT_50NS       8

// Gpio Defines
#define PI_LOW          0
#define PI_HIGH         1
#define GPIO4           4

#define LED_SHIFT_3V3   14
#define D1_GPIO1        18
#define D1_XSHUT        16
#define D2_GPIO1        7
#define D2_XSHUT        8
#define D3_GPIO1        1
#define D3_XSHUT        23
#define D4_GPIO1        20
#define D4_XSHUT        12
#define D5_GPIO1        24
#define D5_XSHUT        25
#define D6_GPIO1        21
#define D6_XSHUT        15

typedef enum
{
    SENSOR1,
    SENSOR2,
    SENSOR3,
    SENSOR4,
    SENSOR5,
    SENSOR6
}sensorID;

// Structs

/**
 * sensorValues structure contains status and distance value of a sensor
 **/
typedef struct
{
    uint8_t rangeStatus; // range status 0 means data ready.
    uint16_t currentDistance_mm;
    uint16_t lastDistance_mm;
    int currentVelocity;
    int averageVelocity;
}sensorValues;




