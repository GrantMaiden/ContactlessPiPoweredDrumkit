/****************************** Module Header ******************************\
Module Name:    controller.h
Project:        btb
Author:         Grant Maiden
Description:    System controller state machine
\***************************************************************************/

#define LIGHT_HIT       10
#define MEDIUM_HIT      30
#define HARD_HIT        50

#define VELOCITY_FACTOR 8

#define SENSOR1_6_DIRECTION         -1
#define SENSOR2_3_4_5_DIRECTION     1

/**
 * Controller State Machine enums for state lookup
 **/
enum controllerState
{
    RESET,
    WAIT1,
    POLLING1,
};

/**
 * sensorValue structure contains status and distance value of a sensor
 **/
typedef struct
{
    sensorID sensor;
    int hitStrength;
}sensorHit;

/**
 * initialize controller globals and states
 **/
void controllerInit();

/**
 * Controller State Machine. Handles all system states, inputs, and outputs.
 **/
void controllerSM();

/**
 * calculates most recent velocity of each sensor
 **/
static void controllerVelocityCalc();


/**
 * updates sensor value
 * \param senseValue- sensorValues struct
 * \param senseID- id of current sensor
 **/
void controllerUpdateSensorValue(sensorValues senseValue, sensorID id);

/**
 * Checks to see if sensor values are ready. if they are, changes status value to not ready.
 * \returns bool- returns true if sensors are ready.
 **/
static bool controllerSensorsReady();

/**
 * updates the distances and Velocity that are stored in each sensorValues object.
 **/
static void controllerUpdateDistanceVelocity();

/**
 * computes if a hit was detected
 **/
static void controllerHitDetection();

/**
 * Requests a sound is played from the corresponding sensor at the input detectionValue level
 * \param id- sensorID
 * \param detectionValue- strength that sensor has been struck
 **/
static void controllerSendSound(sensorID id, int detectionValue);
