/****************************** Module Header ******************************\
Module Name:    controller.cpp
Project:        btb
Author:         Grant Maiden
Description:    System controller state machine
\***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>
#include <sys/time.h>
#include <chrono>
#include <iostream>

#include "defines.h"
#include "controller.h"
#include "ranging.h"
#include "sound.h"

controllerState currentState = POLLING1;
controllerState nextState = currentState;

sensorValues sens1Values;
sensorValues sens2Values;
sensorValues sens3Values;
sensorValues sens4Values;
sensorValues sens5Values;
sensorValues sens6Values;

static int * averageVelArrSens1 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
static int * averageVelArrSens2 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
static int * averageVelArrSens3 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
static int * averageVelArrSens4 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
static int * averageVelArrSens5 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();
static int * averageVelArrSens6 = new int[PAST_AVERAGE_VELOCITY_ARR_SIZE]();

static int averageVelArrIndex = 0;
static bool enableSounds = false;

struct timespec clockObj;
long int stopTimeGesture;

long int timeoutEndTimeSensor1;
long int timeoutEndTimeSensor2;
long int timeoutEndTimeSensor3;
long int timeoutEndTimeSensor4;
long int timeoutEndTimeSensor5;
long int timeoutEndTimeSensor6;

static bool hitDetectedRecently = false;

bool gesture_detected_sensor5 = false;
bool gesture_detected_sensor4 = false;
bool gesture_detected_sensor3 = false;
bool gesture_detected_sensor2 = false;

/**********************************************\
Function Name:  controllerInit
Input Args:     none
Output Args:    void
Description:    initialize controller globals and states
/**********************************************/
void controllerInit()
{
    currentState = RESET;
    nextState = currentState;
    averageVelArrIndex = 0;
    enableSounds = false;

    gesture_detected_sensor5 = false;
    gesture_detected_sensor4 = false;
    gesture_detected_sensor3 = false;
    gesture_detected_sensor2 = false;

    timeoutEndTimeSensor1 = 0;
    timeoutEndTimeSensor2 = 0;
    timeoutEndTimeSensor3 = 0;
    timeoutEndTimeSensor4 = 0;
    timeoutEndTimeSensor5 = 0;
    timeoutEndTimeSensor6 = 0;

    hitDetectedRecently = false;

    stopTimeGesture = 0;
}

/**********************************************\
Function Name:  controllerUpdateState
Input Args:     none
Output Args:    void
Description:    update controller state
/**********************************************/
void controllerUpdateState(controllerState newState)
{
    nextState = newState;
}

/**********************************************\
Function Name:  controllerSM
Input Args:     none
Output Args:    void
Description:    Controller State Machine. Handles all system states, inputs, and outputs.
/**********************************************/
void controllerSM()
{
    // Update current state from nextState
    currentState = nextState;
    switch(currentState)
    {
        case RESET:
            controllerInit();
            nextState = WAIT1;
            // TODO: add initDistanceSensors
            break;
        case WAIT1:
            //if(ledGetCurrentState() == MAINSTATE) // TODO: IMPLEMENT FUNTION AND CORRECT STATE NAME
            {
                nextState = POLLING1;
                //soundPlay(introSound); // TODO: implement Soundplay function
            }
            break;
        case POLLING1:
            if(controllerSensorsReady())
            {
                controllerVelocityCalc();
                //controllerPrintSensorAverageVelocity();
                if(controllerHitDetection())
                {
                    printf("Hit Detected\n");
                    hitDetectedRecently = true;
                }
                controllerUpdateDistance();
                if (controllerGestureDetect(hitDetectedRecently))
                {
                    printf("Swipe Detect!\n");
                    enableSounds = ~enableSounds;
                }
            }
            break;
        case TEST_DISTANCE_SENSORS:
            if(controllerSensorsReady())
            {
                controllerVelocityCalc();
                controllerUpdateDistance();
                controllerPrintSensorCurrentDistance();
            }
            break;
        default:
            nextState = RESET;
            break;
    }
}

/**********************************************\
Function Name:  controllerSendSound
Input Args:     id - sensorID which sensor is sending sound
                detectionValue - int level of hit
Output Args:    void
Description:    computes if a hit was detected
/**********************************************/
static void controllerSendSound(sensorID id, int detectionValue)
{
    printf("Strength: %i\n", detectionValue);
//    if (detectionValue > HARD_HIT)
//        soundTest1(); // TODO: soundPlay(id, HARD_HIT);
//    else if (detectionValue > MEDIUM_HIT)
//        soundTest1(); // TODO: soundPlay(id, MEDIUM_HIT);
//    else if (detectionValue > LIGHT_HIT)
//        soundTest1(); // TODO: soundPlay(id, LIGHT_HIT);

}

/**********************************************\
Function Name:  controllerVelocityCalc
Input Args:     none
Output Args:    void
Description:    calculates most recent velocity of each sensor
/**********************************************/
static void controllerVelocityCalc()
{
    if (sens1Values.lastDistance_mm != 0 && sens1Values.currentDistance_mm != 0)
        sens1Values.currentVelocity = sens1Values.currentDistance_mm - sens1Values.lastDistance_mm;
    else
    {
        sens1Values.currentVelocity = 0;
        sens1Values.averageVelocity = 0;
    }

    if (sens2Values.lastDistance_mm != 0 && sens2Values.currentDistance_mm != 0)
        sens2Values.currentVelocity = sens2Values.currentDistance_mm - sens2Values.lastDistance_mm;
    else
    {
        sens2Values.currentVelocity = 0;
        sens2Values.averageVelocity = 0;
    }

    if (sens3Values.lastDistance_mm != 0 && sens3Values.currentDistance_mm != 0)
        sens3Values.currentVelocity = sens3Values.currentDistance_mm - sens3Values.lastDistance_mm;
    else
    {
        sens3Values.currentVelocity = 0;
        sens3Values.averageVelocity = 0;
    }

    if (sens4Values.lastDistance_mm != 0 && sens4Values.currentDistance_mm != 0)
        sens4Values.currentVelocity = sens4Values.currentDistance_mm - sens4Values.lastDistance_mm;
    else
    {
        sens4Values.currentVelocity = 0;
        sens4Values.averageVelocity = 0;
    }

    if (sens5Values.lastDistance_mm != 0 && sens5Values.currentDistance_mm != 0)
        sens5Values.currentVelocity = sens5Values.currentDistance_mm - sens5Values.lastDistance_mm;
    else
    {
        sens5Values.currentVelocity = 0;
        sens5Values.averageVelocity = 0;
    }

    if (sens6Values.lastDistance_mm != 0 && sens6Values.currentDistance_mm != 0)
        sens6Values.currentVelocity = sens6Values.currentDistance_mm - sens6Values.lastDistance_mm;
    else
    {
        sens6Values.currentVelocity = 0;
        sens6Values.averageVelocity = 0;
    }

    sens1Values.averageVelocity = sens1Values.averageVelocity + (sens1Values.currentVelocity - sens1Values.averageVelocity) / VELOCITY_FACTOR;

    sens2Values.averageVelocity = sens2Values.averageVelocity + (sens2Values.currentVelocity - sens2Values.averageVelocity) / VELOCITY_FACTOR;

    sens3Values.averageVelocity = sens3Values.averageVelocity + (sens3Values.currentVelocity - sens3Values.averageVelocity) / VELOCITY_FACTOR;

    sens4Values.averageVelocity = sens4Values.averageVelocity + (sens4Values.currentVelocity - sens4Values.averageVelocity) / VELOCITY_FACTOR;

    sens5Values.averageVelocity = sens5Values.averageVelocity + (sens5Values.currentVelocity - sens5Values.averageVelocity) / VELOCITY_FACTOR;

    sens6Values.averageVelocity = sens6Values.averageVelocity + (sens6Values.currentVelocity - sens6Values.averageVelocity) / VELOCITY_FACTOR;

    averageVelArrSens1[averageVelArrIndex] = sens1Values.averageVelocity;
    averageVelArrSens2[averageVelArrIndex] = sens2Values.averageVelocity;
    averageVelArrSens3[averageVelArrIndex] = sens3Values.averageVelocity;
    averageVelArrSens4[averageVelArrIndex] = sens4Values.averageVelocity;
    averageVelArrSens5[averageVelArrIndex] = sens5Values.averageVelocity;
    averageVelArrSens6[averageVelArrIndex] = sens6Values.averageVelocity;

    averageVelArrIndex = averageVelArrIndex + 1;
    if (averageVelArrIndex >= PAST_AVERAGE_VELOCITY_ARR_SIZE)
        averageVelArrIndex = 0;
}

/**********************************************\
Function Name:  controllerHitDetection
Input Args:     none
Output Args:    bool
Description:    computes if a hit was detected
/**********************************************/
static bool controllerHitDetection()
{
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    int currentTimeMs = (start.tv_sec * 1000000 + start.tv_usec) / 1000;
    //printf("CurrTime: %u\n", currentTimeMs);
    int detectionValue = 0;
    bool returnVal = false;
    if (sens1Values.averageVelocity * SENSOR1_6_DIRECTION > 0 && sens1Values.currentVelocity * SENSOR1_6_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR1, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor1 < currentTimeMs && detectionValue * SENSOR1_6_DIRECTION > LIGHT_HIT)
    {
        controllerSendSound(sensorID::SENSOR1, detectionValue);
        timeoutEndTimeSensor1 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        returnVal = true;
    }
    detectionValue = 0;
    if (sens2Values.averageVelocity * SENSOR2_3_4_5_DIRECTION > 0 && sens2Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR2, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor2 < currentTimeMs && detectionValue * SENSOR2_3_4_5_DIRECTION > LIGHT_HIT)
    {
        timeoutEndTimeSensor2 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        controllerSendSound(sensorID::SENSOR2, detectionValue);
        returnVal = true;
    }
    detectionValue = 0;
    if (sens3Values.averageVelocity * SENSOR2_3_4_5_DIRECTION > 0 && sens3Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR3, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor3 < currentTimeMs && detectionValue * SENSOR2_3_4_5_DIRECTION > LIGHT_HIT)
    {
        timeoutEndTimeSensor3 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        controllerSendSound(sensorID::SENSOR3, detectionValue);
        returnVal = true;
    }
    detectionValue = 0;
    if (sens4Values.averageVelocity * SENSOR2_3_4_5_DIRECTION > 0 && sens4Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR4, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor4 < currentTimeMs && detectionValue * SENSOR2_3_4_5_DIRECTION > LIGHT_HIT)
    {
        timeoutEndTimeSensor4 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        controllerSendSound(sensorID::SENSOR4, detectionValue);
        returnVal = true;
    }
    detectionValue = 0;
    if (sens5Values.averageVelocity * SENSOR2_3_4_5_DIRECTION > 0 && sens5Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR5, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor5 < currentTimeMs && detectionValue * SENSOR2_3_4_5_DIRECTION > LIGHT_HIT)
    {
        timeoutEndTimeSensor5 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        controllerSendSound(sensorID::SENSOR5, detectionValue);
        returnVal = true;
    }
    detectionValue = 0;
    if (sens6Values.averageVelocity * SENSOR2_3_4_5_DIRECTION > 0 && sens6Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR6, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor6 < currentTimeMs && detectionValue * SENSOR1_6_DIRECTION > LIGHT_HIT)
    {
        timeoutEndTimeSensor6 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        controllerSendSound(sensorID::SENSOR6, detectionValue);
        returnVal = true;
    }

    return returnVal;
}

/**********************************************\
Function Name:  getPeakVelocity
Input Args:     sensorID id
                samplesToSearch
Output Args:    int
Description:    return peak Velocity
/**********************************************/
static int getPeakVelocity(sensorID id, int samplesToSearch)
{
    int * velArr;
    switch (id)
    {
        case sensorID::SENSOR1:
            velArr = averageVelArrSens1;
            break;
        case sensorID::SENSOR2:
            velArr = averageVelArrSens2;
            break;
        case sensorID::SENSOR3:
            velArr = averageVelArrSens3;
            break;
        case sensorID::SENSOR4:
            velArr = averageVelArrSens4;
            break;
        case sensorID::SENSOR5:
            velArr = averageVelArrSens5;
            break;
        case sensorID::SENSOR6:
            velArr = averageVelArrSens6;
            break;
    }
    int largestNeg = 0;
    int largestPos = 0;
    int largestValue;
    for (int i=0; i<PAST_AVERAGE_VELOCITY_ARR_SIZE; i++)
    {
        if (velArr[i] < 0 && largestNeg > velArr[i])
            largestNeg = velArr[i];
        if (velArr[i] > 0 && largestPos < velArr[i])
            largestPos = velArr[i];
    }
    //printf("largestPos: %i\n", largestPos);
    //printf("largestNeg: %i\n", largestNeg);
    if (largestNeg*-1 > largestPos)
            return largestNeg;
        else
            return largestPos;
}

/**********************************************\
Function Name:  controllerUpdateSensorValue
Input Args:     sensorValue senseValue
                sensorID id
Output Args:    void
Description:    updates sensor value
/**********************************************/
void controllerUpdateSensorValue(sensorValues senseValue, sensorID id)
{
    switch (id)
    {
        case sensorID::SENSOR1:
            sens1Values.rangeStatus = senseValue.rangeStatus;
            if (senseValue.currentDistance_mm > DISTANCE_UPPER_THRESHOLD)
                senseValue.currentDistance_mm = 0;
            sens1Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR2:
            sens2Values.rangeStatus = senseValue.rangeStatus;
            if (senseValue.currentDistance_mm > DISTANCE_UPPER_THRESHOLD)
                senseValue.currentDistance_mm = 0;
            sens2Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR3:
            sens3Values.rangeStatus = senseValue.rangeStatus;
            if (senseValue.currentDistance_mm > DISTANCE_UPPER_THRESHOLD)
                senseValue.currentDistance_mm = 0;
            sens3Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR4:
            sens4Values.rangeStatus = senseValue.rangeStatus;
            if (senseValue.currentDistance_mm > DISTANCE_UPPER_THRESHOLD)
                senseValue.currentDistance_mm = 0;
            sens4Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR5:
            sens5Values.rangeStatus = senseValue.rangeStatus;
            if (senseValue.currentDistance_mm > DISTANCE_UPPER_THRESHOLD)
                senseValue.currentDistance_mm = 0;
            sens5Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR6:
            sens6Values.rangeStatus = senseValue.rangeStatus;
            if (senseValue.currentDistance_mm > DISTANCE_UPPER_THRESHOLD)
                senseValue.currentDistance_mm = 0;
            sens6Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
    }
    //printf("%u | %u | %u | %u | %u | %u\n", sens1Values.rangeStatus, sens2Values.rangeStatus, sens3Values.rangeStatus,sens4Values.rangeStatus,sens5Values.rangeStatus,sens6Values.rangeStatus);
}

/**********************************************\
Function Name:  controllerSensorsReady
Input Args:     none
Output Args:    bool- True if sensors are ready
Description:    Checks to see if sensor values are ready. if they are, changes status value to not ready.
/**********************************************/
static bool controllerSensorsReady()
{
    bool sensorsReadyBool = false;
    if (sens1Values.rangeStatus != 1)
        if (sens2Values.rangeStatus != 1)
            if (sens3Values.rangeStatus != 1)
                if (sens4Values.rangeStatus != 1)
                    if (sens5Values.rangeStatus != 1)
                        if (sens6Values.rangeStatus != 1)
                        {
                            sensorsReadyBool = true;
                        }
    if(sensorsReadyBool)
    {
        sens1Values.rangeStatus = 1;
        sens2Values.rangeStatus = 1;
        sens3Values.rangeStatus = 1;
        sens4Values.rangeStatus = 1;
        sens5Values.rangeStatus = 1;
        sens6Values.rangeStatus = 1;
    }


    return sensorsReadyBool;
}

/**********************************************\
Function Name:  controllerUpdateDistance
Input Args:     none
Output Args:    void
Description:    updates the distances that are stored in each sensorValues object.
/**********************************************/
static void controllerUpdateDistance()
{
    sens1Values.lastDistance_mm = sens1Values.currentDistance_mm;

    sens2Values.lastDistance_mm = sens2Values.currentDistance_mm;

    sens3Values.lastDistance_mm = sens3Values.currentDistance_mm;

    sens4Values.lastDistance_mm = sens4Values.currentDistance_mm;

    sens5Values.lastDistance_mm = sens5Values.currentDistance_mm;

    sens6Values.lastDistance_mm = sens6Values.currentDistance_mm;
}

/**********************************************\
Function Name:  controllerPrintSensorCurrentDistance
Input Args:     none
Output Args:    void
Description:    Prints sensor data to console
/**********************************************/
static void controllerPrintSensorCurrentDistance()
{
    printf("%u\t%u\t%u\t%u\t%u\t%u\n",
         sens1Values.currentDistance_mm,
         sens2Values.currentDistance_mm,
         sens3Values.currentDistance_mm,
         sens4Values.currentDistance_mm,
         sens5Values.currentDistance_mm,
         sens6Values.currentDistance_mm);
}

/**********************************************\
Function Name:  controllerPrintSensorAverageVelocity
Input Args:     none
Output Args:    void
Description:    Prints sensor data to console
/**********************************************/
static void controllerPrintSensorAverageVelocity()
{

    printf("%i\t%i\t%i\t%i\t%i\t%i\n",
         sens1Values.averageVelocity,
         sens2Values.averageVelocity,
         sens3Values.averageVelocity,
         sens4Values.averageVelocity,
         sens5Values.averageVelocity,
         sens6Values.averageVelocity);
}

/**********************************************\
Function Name:  controllerGestureDetect
Input Args:     bool hitDetectedRecently
Output Args:    bool
Description:    Detects Gestures
/**********************************************/
static bool controllerGestureDetect(bool hitDetectedRecently)
{

    int loop = 0;
    int currentIndex = 0;
    int lastVal = 0;
    int checkVal = 0;
    int loopVal = 0;
    bool returnVal = false;
    clock_gettime(CLOCK_REALTIME, &clockObj);
    long int currentTime = clockObj.tv_sec;
    currentIndex = averageVelArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens5[currentIndex] != 0)
    {
        currentIndex = averageVelArrIndex - 1;
        if (currentIndex < 0)
            currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
        checkVal = averageVelArrSens5[currentIndex];
        loopVal = checkVal;
        while(loop<GESTURE_THRESHOLD_NUM_SAMPLE_TRUE)
        {
            if(loopVal == 0 || loopVal < checkVal - GESTURE_THRESHOLD_VELOCITY || checkVal + GESTURE_THRESHOLD_VELOCITY < loopVal)
            {
                goto SENS4;
            }
            currentIndex = currentIndex - 1;
            if (currentIndex < 0)
                currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
            loop = loop + 1;
            loopVal = averageVelArrSens5[currentIndex];
        }
        if (currentTime > stopTimeGesture)
        {
            clock_gettime(CLOCK_REALTIME, &clockObj);
            stopTimeGesture = clockObj.tv_sec + GESTURE_TIME_LIMIT;
            gesture_detected_sensor5 = true;
            //printf("Gesture Detect S5\n");
        }
    }
SENS4:
    currentIndex = averageVelArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens4[currentIndex] != 0)
    {
        currentIndex = averageVelArrIndex - 1;
        if (currentIndex < 0)
            currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
        checkVal = averageVelArrSens4[currentIndex];
        loopVal = checkVal;
        loop = 0;
        //printf("L:%i U:%i\n", checkVal - GESTURE_THRESHOLD_VELOCITY, checkVal + GESTURE_THRESHOLD_VELOCITY);
        while(loop<GESTURE_THRESHOLD_NUM_SAMPLE_TRUE)
        {
            if(loopVal == 0 || loopVal < checkVal - GESTURE_THRESHOLD_VELOCITY || checkVal + GESTURE_THRESHOLD_VELOCITY < loopVal)
            {
               goto SENS3;
            }
            currentIndex = currentIndex - 1;
            if (currentIndex < 0)
                currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
            loop = loop + 1;
            loopVal = averageVelArrSens4[currentIndex];
        }
        gesture_detected_sensor4 = true;
        //printf("Gesture Detect S4\n");
    }
SENS3:
    currentIndex = averageVelArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens3[currentIndex] != 0)
    {
        currentIndex = averageVelArrIndex - 1;
        if (currentIndex < 0)
            currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
        checkVal = averageVelArrSens3[currentIndex];
        loopVal = checkVal;
        loop = 0;
        //printf("L:%i U:%i\n", checkVal - GESTURE_THRESHOLD_VELOCITY, checkVal + GESTURE_THRESHOLD_VELOCITY);
        while(loop<GESTURE_THRESHOLD_NUM_SAMPLE_TRUE)
        {
            if(loopVal == 0 || loopVal < checkVal - GESTURE_THRESHOLD_VELOCITY || checkVal + GESTURE_THRESHOLD_VELOCITY < loopVal)
            {
                goto SENS2;
            }
            currentIndex = currentIndex - 1;
            if (currentIndex < 0)
                currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
            loop = loop + 1;
            loopVal = averageVelArrSens3[currentIndex];
        }
        gesture_detected_sensor3 = true;
        //printf("Gesture Detect S3\n");
    }
SENS2:
    currentIndex = averageVelArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens2[currentIndex] != 0)
    {
        currentIndex = averageVelArrIndex - 1;
            if (currentIndex < 0)
                currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
        checkVal = averageVelArrSens2[currentIndex];
        loopVal = checkVal;
        loop = 0;
        //printf("L:%i U:%i\n", checkVal - GESTURE_THRESHOLD_VELOCITY, checkVal + GESTURE_THRESHOLD_VELOCITY);
        while(loop<GESTURE_THRESHOLD_NUM_SAMPLE_TRUE)
        {
            if(loopVal == 0 || loopVal < checkVal - GESTURE_THRESHOLD_VELOCITY || checkVal + GESTURE_THRESHOLD_VELOCITY < loopVal)
            {
                goto LOGIC;
            }
            currentIndex = currentIndex - 1;
            if (currentIndex < 0)
                currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
            loop = loop + 1;
            loopVal = averageVelArrSens2[currentIndex];
        }
        gesture_detected_sensor2 = true;
        //printf("Gesture Detect S2\n");
    }
LOGIC:
    if (currentTime > stopTimeGesture || hitDetectedRecently)
    {
        gesture_detected_sensor5 = false;
        gesture_detected_sensor4 = false;
        gesture_detected_sensor3 = false;
        gesture_detected_sensor2 = false;
        hitDetectedRecently = false;
    }

    if (gesture_detected_sensor5 && gesture_detected_sensor4 && gesture_detected_sensor3 && gesture_detected_sensor2)
    {
        gesture_detected_sensor5 = false;
        gesture_detected_sensor4 = false;
        gesture_detected_sensor3 = false;
        gesture_detected_sensor2 = false;
        returnVal = true;
    }

    return returnVal;

}

