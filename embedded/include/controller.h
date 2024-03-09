/****************************** Module Header ******************************\
Module Name:    controller.h
Project:        btb
Author:         Grant Maiden
Description:    System controller state machine
\***************************************************************************/

#define LIGHT_HIT       3
#define MEDIUM_HIT      7
#define HARD_HIT        10

// Distance Filtering
#define DISTANCE_UPPER_THRESHOLD            600

// Velocity array control
#define VELOCITY_FACTOR_AVG                 3
#define PAST_AVERAGE_VELOCITY_ARR_SIZE      16

// Hit Detection
#define HIT_DETECT_NOISE_CHECKING                   true
#define HIT_DETECT_NUM_SAMPLES_MIN                  8
#define DRUM_INTERVAL_TIMEOUT_MS                    120
#define HIT_DETECT_INSTANEOUS_VELOCITY_LIMIT        100
#define HIT_DETECT_AVG_VELOCITY_CEILING             25
#define HIT_DETECT_MIN_SEQUENTIAL_VELOCITY_SAMPLES  6

// Sensor Detection Direction (-1 = towards sensor, 1 = away from sensor)
#define SENSOR1_6_DIRECTION                 1
#define SENSOR2_3_4_5_DIRECTION             -1

// Gestures
#define GESTURE_THRESHOLD_VELOCITY          6
#define GESTURE_THRESHOLD_NUM_SAMPLE_TRUE   4
#define GESTURE_TIME_LIMIT                  1

/**
 * Controller State Machine enums for state lookup
 **/
typedef enum
{
    RESET,
    WAIT1,
    POLLING1,
    TEST_DISTANCE_SENSORS,
}controllerState;

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
 * update controller state
 **/
void controllerUpdateState(controllerState newState);

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
 * updates the distances that are stored in each sensorValues object.
 **/
static void controllerUpdateDistance();

/**
 * computes if a hit was detected
 * \returns bool- returns true if hit was detected.
 **/
static bool controllerHitDetection();

/**
 * Requests a sound is played from the corresponding sensor at the input detectionValue level
 * \param id- sensorID
 * \param detectionValue- strength that sensor has been struck
 **/
static void controllerSendSound(sensorID id, int detectionValue);

/**
 * return peak Velocity
 * \param id- sensorID
 * \param samplesToSearch- number of previous samples to search
 * \param noiseCheckingEn- checks for noise with additional arguments. returns zero if noise check fails.
 * \returns int - peak velocity
 **/
static int getPeakVelocity(sensorID id, int samplesToSearch, bool noiseCheckingEn = HIT_DETECT_NOISE_CHECKING);

/**
 * Prints sensor data to console
 **/
static void controllerPrintSensorCurrentDistance();

/**
 * Prints sensor velocity data to console
 **/
static void controllerPrintSensorAverageVelocity();

/**
 * Detects Gestures
 * \returns bool- returns true if gesture detected
 **/
static bool controllerGestureDetect();
