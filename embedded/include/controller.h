/****************************** Module Header ******************************\
Module Name:    controller.h
Project:        btb
Author:         Grant Maiden
Description:    System controller state machine
\***************************************************************************/
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <pigpio.h>

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


using namespace std;
class Controller
{
    public:
        /**
         * Controller State Machine. Handles all system states, inputs, and outputs.
         **/
        void primaryStateMachine();

        /**
         * update controller state
         **/
        void updateState(controllerState newState);

        /**
         * updates sensor value
         * \param senseValue- sensorValues struct
         * \param senseID- id of current sensor
         **/
        void updateSensorValue(sensorValues senseValue, sensorID id);

//        /**
//         * isrCallback
//         **/
//        static void isrCallback(void*obj);
//
//        /**
//         * Callback that is triggered on DistanceSensors Interrupt Falling Edge.
//         * \param gpio - int gpio Triggered Gpio number
//         * \param level - int level GPIO input level at time of ISR
//         * \param tick - uint32_t trigger time in microseconds.
//         **/
//        static void rangingISRCallback(int gpio, int level, uint32_t tick);

    private:
        controllerState currentState = RESET;
        controllerState nextState = currentState;

        sensorValues sens1Values;
        sensorValues sens2Values;
        sensorValues sens3Values;
        sensorValues sens4Values;
        sensorValues sens5Values;
        sensorValues sens6Values;

        int * averageVelArrSens1 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * averageVelArrSens2 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * averageVelArrSens3 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * averageVelArrSens4 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * averageVelArrSens5 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * averageVelArrSens6 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();

        int * currentVelArrSens1 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * currentVelArrSens2 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * currentVelArrSens3 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * currentVelArrSens4 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * currentVelArrSens5 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
        int * currentVelArrSens6 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();

        int averageVelNextArrIndex = 0;
        bool enableSounds = false;

        struct timespec clockObj;
        unsigned stopTimeGesture;

        uint timeoutEndTimeSensor1;
        uint timeoutEndTimeSensor2;
        uint timeoutEndTimeSensor3;
        uint timeoutEndTimeSensor4;
        uint timeoutEndTimeSensor5;
        uint timeoutEndTimeSensor6;

        bool hitDetectedRecently = false;

        bool gesture_detected_sensor5 = false;
        bool gesture_detected_sensor4 = false;
        bool gesture_detected_sensor3 = false;
        bool gesture_detected_sensor2 = false;
        /**
         * initialize controller globals and states
         **/
        void initialize();


        /**
         * calculates most recent velocity of each sensor
         **/
        void velocityCalc();

        /**
         * Checks to see if sensor values are ready. if they are, changes status value to not ready.
         * \returns bool- returns true if sensors are ready.
         **/
        bool sensorsReady();

        /**
         * updates the distances that are stored in each sensorValues object.
         **/
        void updateDistance();

        /**
         * computes if a hit was detected
         * \returns bool- returns true if hit was detected.
         **/
        bool hitDetection();

        /**
         * Requests a sound is played from the corresponding sensor at the input detectionValue level
         * \param id- sensorID
         * \param detectionValue- strength that sensor has been struck
         **/
        void sendSound(sensorID id, int detectionValue);

        /**
         * return peak Velocity
         * \param id- sensorID
         * \param samplesToSearch- number of previous samples to search
         * \param noiseCheckingEn- checks for noise with additional arguments. returns zero if noise check fails.
         * \returns int - peak velocity
         **/
        int getPeakVelocity(sensorID id, int samplesToSearch, bool noiseCheckingEn = HIT_DETECT_NOISE_CHECKING);

        /**
         * Prints sensor data to console
         **/
        void printSensorCurrentDistance();

        /**
         * Prints sensor velocity data to console
         **/
        void printSensorAvgVelocity();

        /**
         * Detects Gestures
         * \returns bool- returns true if gesture detected
         **/
        bool gestureDetect();

        /**
         * Initialize Leds
         **/
        void initLeds();

//        /**
//         * Initialize Interrupts
//         **/
//        void initInterrupts();

        /**
         * Parses Command line arguements. Command line inputs are used primarily for unit tests
         * \param argc - int containging the number of command line arguements
         * \param *argv[] - char* to an array of characters containing the input commands
         **/
        void parseCommandLine(int argc,char *argv[]);

        /**
         * Runs command line arguements
         * \param argv - char array input of characters containing the input command
         **/
        void runCommandLine(char *argv[]);

};

#endif // CONTROLLER_H
