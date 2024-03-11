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



/**********************************************\
Function Name:  initialize
Input Args:     none
Output Args:    void
Description:    initialize controller globals and states
/**********************************************/
void Controller::initialize()
{
    currentState = RESET;
    nextState = currentState;
    averageVelNextArrIndex = 0;
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
Function Name:  updateState
Input Args:     none
Output Args:    void
Description:    update controller state
/**********************************************/
void Controller::updateState(controllerState newState)
{
    nextState = newState;
}

/**********************************************\
Function Name:  primaryStateMachine
Input Args:     none
Output Args:    void
Description:    Controller State Machine. Handles all system states, inputs, and outputs.
/**********************************************/
void Controller::primaryStateMachine()
{
    // Update current state from nextState
    currentState = nextState;
    switch(currentState)
    {
        case RESET:
            initialize();
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
            if(sensorsReady())
            {
                velocityCalc();
                printSensorAvgVelocity();
                if(hitDetection())
                {
                    hitDetectedRecently = true;
                }
                updateDistance();
                if (gestureDetect())
                {
                    printf("Swipe Detect!\n");
                    enableSounds = ~enableSounds;
                }
            }
            break;
        case TEST_DISTANCE_SENSORS:
            if(sensorsReady())
            {
                velocityCalc();
                updateDistance();
                printSensorCurrentDistance();
            }
            break;
        default:
            nextState = RESET;
            break;
    }
}

/**********************************************\
Function Name:  sendSound
Input Args:     id - sensorID which sensor is sending sound
                detectionValue - int level of hit
Output Args:    void
Description:    computes if a hit was detected
/**********************************************/
void Controller::sendSound(sensorID id, int detectionValue)
{
    printf("Hit Detected! Strength: %i Sensor: %i\n", detectionValue, id+1);
//    if (detectionValue > HARD_HIT)
//        soundTest1(); // TODO: soundPlay(id, HARD_HIT);
//    else if (detectionValue > MEDIUM_HIT)
//        soundTest1(); // TODO: soundPlay(id, MEDIUM_HIT);
//    else if (detectionValue > LIGHT_HIT)
//        soundTest1(); // TODO: soundPlay(id, LIGHT_HIT);

}

/**********************************************\
Function Name:  velocityCalc
Input Args:     none
Output Args:    void
Description:    calculates most recent velocity of each sensor
/**********************************************/
void Controller::velocityCalc()
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

    sens1Values.averageVelocity = sens1Values.averageVelocity + (sens1Values.currentVelocity - sens1Values.averageVelocity) / VELOCITY_FACTOR_AVG;

    sens2Values.averageVelocity = sens2Values.averageVelocity + (sens2Values.currentVelocity - sens2Values.averageVelocity) / VELOCITY_FACTOR_AVG;

    sens3Values.averageVelocity = sens3Values.averageVelocity + (sens3Values.currentVelocity - sens3Values.averageVelocity) / VELOCITY_FACTOR_AVG;

    sens4Values.averageVelocity = sens4Values.averageVelocity + (sens4Values.currentVelocity - sens4Values.averageVelocity) / VELOCITY_FACTOR_AVG;

    sens5Values.averageVelocity = sens5Values.averageVelocity + (sens5Values.currentVelocity - sens5Values.averageVelocity) / VELOCITY_FACTOR_AVG;

    sens6Values.averageVelocity = sens6Values.averageVelocity + (sens6Values.currentVelocity - sens6Values.averageVelocity) / VELOCITY_FACTOR_AVG;

    averageVelArrSens1[averageVelNextArrIndex] = sens1Values.averageVelocity;
    averageVelArrSens2[averageVelNextArrIndex] = sens2Values.averageVelocity;
    averageVelArrSens3[averageVelNextArrIndex] = sens3Values.averageVelocity;
    averageVelArrSens4[averageVelNextArrIndex] = sens4Values.averageVelocity;
    averageVelArrSens5[averageVelNextArrIndex] = sens5Values.averageVelocity;
    averageVelArrSens6[averageVelNextArrIndex] = sens6Values.averageVelocity;
    currentVelArrSens1[averageVelNextArrIndex] = sens1Values.currentVelocity;
    currentVelArrSens2[averageVelNextArrIndex] = sens2Values.currentVelocity;
    currentVelArrSens3[averageVelNextArrIndex] = sens3Values.currentVelocity;
    currentVelArrSens4[averageVelNextArrIndex] = sens4Values.currentVelocity;
    currentVelArrSens5[averageVelNextArrIndex] = sens5Values.currentVelocity;
    currentVelArrSens6[averageVelNextArrIndex] = sens6Values.currentVelocity;

    averageVelNextArrIndex = averageVelNextArrIndex + 1;
    if (averageVelNextArrIndex >= PAST_AVERAGE_VELOCITY_ARR_SIZE)
        averageVelNextArrIndex = 0;
}

/**********************************************\
Function Name:  hitDetection
Input Args:     none
Output Args:    bool
Description:    computes if a hit was detected
/**********************************************/
bool Controller::hitDetection()
{
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    uint currentTimeMs = (start.tv_sec * 1000000 + start.tv_usec) / 1000;
    int previousVelAvgIndex = averageVelNextArrIndex - 2;
    if (previousVelAvgIndex < -1)
        previousVelAvgIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 2;
    else if (previousVelAvgIndex < 0)
        previousVelAvgIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;

    //printf("CurrTime: %u\n", currentTimeMs);
    int detectionValue = 0;
    bool returnVal = false;
    if (averageVelArrSens1[previousVelAvgIndex] * SENSOR1_6_DIRECTION > 0 && sens1Values.currentVelocity * SENSOR1_6_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR1, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor1 < currentTimeMs && detectionValue * SENSOR1_6_DIRECTION >= LIGHT_HIT)
    {
        sendSound(sensorID::SENSOR1, detectionValue);
        timeoutEndTimeSensor1 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        returnVal = true;
    }
    detectionValue = 0;
    if (averageVelArrSens2[previousVelAvgIndex] * SENSOR2_3_4_5_DIRECTION > 0 && sens2Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR2, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (detectionValue * SENSOR2_3_4_5_DIRECTION >= LIGHT_HIT)
    {
        //printf("timeoutEndTimeSensor2: %u, currentTimeMs: %u SensorID: %i\n", timeoutEndTimeSensor2, currentTimeMs, sensorID::SENSOR2);
        if(timeoutEndTimeSensor2 < currentTimeMs)
        {
        timeoutEndTimeSensor2 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        sendSound(sensorID::SENSOR2, detectionValue);
        returnVal = true;
        }
    }
    detectionValue = 0;
    if (averageVelArrSens3[previousVelAvgIndex]  * SENSOR2_3_4_5_DIRECTION > 0 && sens3Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR3, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor3 < currentTimeMs && detectionValue * SENSOR2_3_4_5_DIRECTION >= LIGHT_HIT)
    {
        timeoutEndTimeSensor3 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        sendSound(sensorID::SENSOR3, detectionValue);
        returnVal = true;
    }
    detectionValue = 0;
    if (averageVelArrSens4[previousVelAvgIndex]  * SENSOR2_3_4_5_DIRECTION > 0 && sens4Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR4, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor4 < currentTimeMs && detectionValue * SENSOR2_3_4_5_DIRECTION >= LIGHT_HIT)
    {
        timeoutEndTimeSensor4 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        sendSound(sensorID::SENSOR4, detectionValue);
        returnVal = true;
    }
    detectionValue = 0;
    if (averageVelArrSens5[previousVelAvgIndex]  * SENSOR2_3_4_5_DIRECTION > 0 && sens5Values.currentVelocity * SENSOR2_3_4_5_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR5, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor5 < currentTimeMs && detectionValue * SENSOR2_3_4_5_DIRECTION >= LIGHT_HIT)
    {
        timeoutEndTimeSensor5 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        sendSound(sensorID::SENSOR5, detectionValue);
        returnVal = true;
    }
    detectionValue = 0;
    if (averageVelArrSens6[previousVelAvgIndex]  * SENSOR1_6_DIRECTION > 0 && sens6Values.currentVelocity * SENSOR1_6_DIRECTION < 0)
        detectionValue = getPeakVelocity(sensorID::SENSOR6, PAST_AVERAGE_VELOCITY_ARR_SIZE);
    if (timeoutEndTimeSensor6 < currentTimeMs && detectionValue * SENSOR1_6_DIRECTION >= LIGHT_HIT)
    {
        timeoutEndTimeSensor6 = currentTimeMs + DRUM_INTERVAL_TIMEOUT_MS;
        sendSound(sensorID::SENSOR6, detectionValue);
        returnVal = true;
    }

    return returnVal;
}

/**********************************************\
Function Name:  getPeakVelocity
Input Args:     sensorID id
                samplesToSearch
                noiseCheckingEn - checks for noise with additional arguments. returns zero if noise check fails.
Output Args:    int
Description:    return peak Velocity
/**********************************************/
int Controller::getPeakVelocity(sensorID id, int samplesToSearch, bool noiseCheckingEn)
{
    int * velArr;
    int * currentVelArr;
    int returnSign = 0;
    switch (id)
    {
        case sensorID::SENSOR1:
            velArr = averageVelArrSens1;
            currentVelArr = currentVelArrSens1;
            returnSign = SENSOR1_6_DIRECTION;
            break;
        case sensorID::SENSOR2:
            velArr = averageVelArrSens2;
            currentVelArr = currentVelArrSens2;
            returnSign = SENSOR2_3_4_5_DIRECTION;
            break;
        case sensorID::SENSOR3:
            velArr = averageVelArrSens3;
            currentVelArr = currentVelArrSens3;
            returnSign = SENSOR2_3_4_5_DIRECTION;
            break;
        case sensorID::SENSOR4:
            velArr = averageVelArrSens4;
            currentVelArr = currentVelArrSens4;
            returnSign = SENSOR2_3_4_5_DIRECTION;
            break;
        case sensorID::SENSOR5:
            velArr = averageVelArrSens5;
            currentVelArr = currentVelArrSens5;
            returnSign = SENSOR2_3_4_5_DIRECTION;
            break;
        case sensorID::SENSOR6:
            velArr = averageVelArrSens6;
            currentVelArr = currentVelArrSens6;
            returnSign = SENSOR1_6_DIRECTION;
            break;
    }
    int largestNeg = 0;
    int largestPos = 0;
    int largestValue;
    int peakValueIndexPos = 0;
    int peakValueIndexNeg = 0;
    int numSampleNonZero = 0;
    int currentVelPeakPos = 0;
    int currentVelPeakNeg = 0;
    for (int i=0; i<PAST_AVERAGE_VELOCITY_ARR_SIZE; i++)
    {
        if (largestNeg > velArr[i])
        {
            largestNeg = velArr[i];
            peakValueIndexNeg = i;
        }
        if (largestPos < velArr[i])
        {
            largestPos = velArr[i];
            peakValueIndexPos = i;
        }
        if (currentVelPeakNeg > currentVelArr[i])
        {
            currentVelPeakNeg = currentVelArr[i];
        }
        if (currentVelPeakPos < currentVelArr[i])
        {
            currentVelPeakPos = currentVelArr[i];
        }

        if (velArr[i] != 0)
            numSampleNonZero = numSampleNonZero+1;
    }
    //printf("largestPos: %i\n", largestPos);
    //printf("largestNeg: %i\n", largestNeg);
    int currentVelArrIndex = averageVelNextArrIndex - 1;
    if (currentVelArrIndex < 0)
        currentVelArrIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE -1;
    int lastVelArrIndex = currentVelArrIndex - 1;
    if (lastVelArrIndex < 0)
        currentVelArrIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE -1;
    int currentVelocityMax = 0;
    if (currentVelPeakNeg * -1 >= currentVelPeakPos)
        currentVelocityMax = currentVelPeakNeg * -1;
    else
        currentVelocityMax = currentVelPeakPos;
    int previousIndexPos = peakValueIndexPos - 1;
    if (previousIndexPos < 0)
        previousIndexPos = PAST_AVERAGE_VELOCITY_ARR_SIZE -1;
    int previousIndexNeg = peakValueIndexNeg - 1;
    if (previousIndexNeg < 0)
        previousIndexNeg = PAST_AVERAGE_VELOCITY_ARR_SIZE -1;
    int sequentialVelCount = 0;
    int sequentialVelIndex = lastVelArrIndex;
    if (noiseCheckingEn)
    {
        for(int i=0; i<HIT_DETECT_MIN_SEQUENTIAL_VELOCITY_SAMPLES; i++)
        {
            if (velArr[sequentialVelIndex] * returnSign <= 0 )
            {
                break;
            }
            sequentialVelIndex = sequentialVelIndex - 1;
            if (sequentialVelIndex < 0)
                sequentialVelIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
            if (currentVelArrIndex == sequentialVelIndex)
            {
                break;
            }
            sequentialVelCount = sequentialVelCount + 1;
        }
        //if (id == sensorID::SENSOR2)
        //  printf("sequentialVelCount %i, Sensor: %i Pos: %i Neg: %i\n", sequentialVelCount, id + 1, largestPos, largestNeg);
        if (sequentialVelCount < HIT_DETECT_MIN_SEQUENTIAL_VELOCITY_SAMPLES)
        {
            largestNeg = 0;
            largestPos = 0;
        }

        if (numSampleNonZero < HIT_DETECT_NUM_SAMPLES_MIN)
        {
            //printf("NonZeroSamples: %i, SensorID: %i\n", numSampleNonZero, id);
            largestNeg = 0;
            largestPos = 0;
        }
        if (velArr[previousIndexNeg] == 0)
            largestNeg = 0;
        if (velArr[previousIndexPos] == 0)
            largestPos = 0;
        //printf("currentVelocityMax: %i\n", currentVelocityMax);
        if(currentVelocityMax > HIT_DETECT_INSTANEOUS_VELOCITY_LIMIT)
        {
            //printf("Velocity Limit Hit With V of : %i\n", currentVelocityMax);
            largestNeg = 0;
            largestPos = 0;
        }
        if(largestNeg * -1 > HIT_DETECT_AVG_VELOCITY_CEILING)
            largestNeg = 0;
        if(largestPos > HIT_DETECT_AVG_VELOCITY_CEILING)
            largestPos = 0;
    }
    //if (id == sensorID::SENSOR2)
        //printf("LargestNeg: %i, LargestPos: %i SensorID: %i\n", largestNeg, largestPos, id);
    if (returnSign < 0)
        return largestNeg;
    else
        return largestPos;
}

/**********************************************\
Function Name:  updateSensorValue
Input Args:     sensorValue senseValue
                sensorID id
Output Args:    void
Description:    updates sensor value
/**********************************************/
void Controller::updateSensorValue(sensorValues senseValue, sensorID id)
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
Function Name:  sensorsReady
Input Args:     none
Output Args:    bool- True if sensors are ready
Description:    Checks to see if sensor values are ready. if they are, changes status value to not ready.
/**********************************************/
bool Controller::sensorsReady()
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
Function Name:  updateDistance
Input Args:     none
Output Args:    void
Description:    updates the distances that are stored in each sensorValues object.
/**********************************************/
void Controller::updateDistance()
{
    sens1Values.lastDistance_mm = sens1Values.currentDistance_mm;

    sens2Values.lastDistance_mm = sens2Values.currentDistance_mm;

    sens3Values.lastDistance_mm = sens3Values.currentDistance_mm;

    sens4Values.lastDistance_mm = sens4Values.currentDistance_mm;

    sens5Values.lastDistance_mm = sens5Values.currentDistance_mm;

    sens6Values.lastDistance_mm = sens6Values.currentDistance_mm;
}

/**********************************************\
Function Name:  printSensorCurrentDistance
Input Args:     none
Output Args:    void
Description:    Prints sensor data to console
/**********************************************/
void Controller::printSensorCurrentDistance()
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
Function Name:  printSensorAvgVelocity
Input Args:     none
Output Args:    void
Description:    Prints sensor data to console
/**********************************************/
void Controller::printSensorAvgVelocity()
{

    printf("%i\t%i\t%i\t%i\t%i\t%i\t\t%i\t%i\t%i\t%i\t%i\t%i\t\t%i\t%i\t%i\t%i\t%i\t%i\t\n",
         sens1Values.averageVelocity,
         sens2Values.averageVelocity,
         sens3Values.averageVelocity,
         sens4Values.averageVelocity,
         sens5Values.averageVelocity,
         sens6Values.averageVelocity,
         sens1Values.currentVelocity,
         sens2Values.currentVelocity,
         sens3Values.currentVelocity,
         sens4Values.currentVelocity,
         sens5Values.currentVelocity,
         sens6Values.currentVelocity,
         sens1Values.currentDistance_mm,
         sens2Values.currentDistance_mm,
         sens3Values.currentDistance_mm,
         sens4Values.currentDistance_mm,
         sens5Values.currentDistance_mm,
         sens6Values.currentDistance_mm);
}

/**********************************************\
Function Name:  gestureDetect
Input Args:     none
Output Args:    bool
Description:    Detects Gestures
/**********************************************/
bool Controller::gestureDetect()
{

    int loop = 0;
    int currentIndex = 0;
    int lastVal = 0;
    int checkVal = 0;
    int loopVal = 0;
    bool returnVal = false;
    clock_gettime(CLOCK_REALTIME, &clockObj);
    long int currentTime = clockObj.tv_sec;
    currentIndex = averageVelNextArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens5[currentIndex] != 0)
    {
        currentIndex = averageVelNextArrIndex - 1;
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
    currentIndex = averageVelNextArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens4[currentIndex] != 0)
    {
        currentIndex = averageVelNextArrIndex - 1;
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
    currentIndex = averageVelNextArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens3[currentIndex] != 0)
    {
        currentIndex = averageVelNextArrIndex - 1;
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
    currentIndex = averageVelNextArrIndex - 1;
    if (currentIndex < 0)
        currentIndex = PAST_AVERAGE_VELOCITY_ARR_SIZE - 1;
    if (averageVelArrSens2[currentIndex] != 0)
    {
        currentIndex = averageVelNextArrIndex - 1;
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

///**********************************************\
//Function Name:  rangingISRCallback()
//Input Args:     int gpio :Triggered Gpio number
//                int level: GPIO input level at time of ISR
//                uint32_t tick: trigger time in microseconds.
//Output Args:    none
//Description:    callback that is triggered on DistanceSensors Interrupt Falling Edge.
///**********************************************/
//void Controller::rangingISRCallback(int gpio, int level, uint32_t tick)
//{
//    //printf("GPIO %d became %d at %d\n", gpio, level, tick); //COMMENT ME OUT WHEN WORKING
//
//    if (level == PI_TIMEOUT)
//    {
//        //printf("GPIO %d returned Interrupt Timeout! Interrupt Exceeded %dms!\nUs tick: %lu\n", gpio, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, tick);
//        ;
//    }
//
//    //printf("GPIO %i\n", gpio);
//
//    sensorValues senseValues;
//    switch(gpio)
//    {
//        case D1_GPIO1:
//            senseValues = rangingGetData(sensorID::SENSOR1);
//            updateSensorValue(senseValues, sensorID::SENSOR1);
//            break;
//        case D2_GPIO1:
//            senseValues = rangingGetData(sensorID::SENSOR2);
//            updateSensorValue(senseValues, sensorID::SENSOR2);
//            break;
//        case D3_GPIO1:
//            senseValues = rangingGetData(sensorID::SENSOR3);
//            updateSensorValue(senseValues, sensorID::SENSOR3);
//            break;
//        case D4_GPIO1:
//            senseValues = rangingGetData(sensorID::SENSOR4);
//            updateSensorValue(senseValues, sensorID::SENSOR4);
//            break;
//        case D5_GPIO1:
//            senseValues = rangingGetData(sensorID::SENSOR5);
//            updateSensorValue(senseValues, sensorID::SENSOR5);
//            break;
//        case D6_GPIO1:
//            senseValues = rangingGetData(sensorID::SENSOR6);
//            updateSensorValue(senseValues, sensorID::SENSOR6);
//            break;
//    }
//}
//
///**********************************************\
//Function Name:  isrCallback()
//Input Args:     none
//Output Args:    none
//Description:
///**********************************************/
//static void isrCallback(void*obj)
//{
//    //static_cast<Controller *>(obj)->rangingISRCallback();
//}
//
///**********************************************\
//Function Name:  initInterrupts()
//Input Args:     none
//Output Args:    none
//Description:    intialize interrupts and timers
///**********************************************/
//void Controller::initInterrupts()
//{
//    gpioSetISRFunc(D1_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, *rangingISRCallback);
//    gpioSetISRFunc(D2_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, typedef void (*rangingISRCallback));
//    gpioSetISRFunc(D3_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
//    gpioSetISRFunc(D4_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
//    gpioSetISRFunc(D5_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
//    gpioSetISRFunc(D6_GPIO1, FALLING_EDGE, DISTANCE_SENSOR_INTERRUPT_TIMEOUT, rangingISRCallback);
//}

///**********************************************\
//Function Name:  parseCommandLine()
//Input Args:     int argc        arguement count
//                char *argv[]    arguement values
//Output Args:    none
//Description:    Parses Command line arguements. Command line inputs are used primarily for unit tests.
///**********************************************/
//void Controller::parseCommandLine(int argc,char *argv[])
//{
//    if (argc > 1)
//    {
//        printf("Found %i additional input arguments\n\n", argc-1);
//        for(int i=1; i<argc; i++)
//        {
//            printf("Attempting to run Command: %s\n", argv[i]);
//            runCommandLine(&argv[i]);
//        }
//        exit(0);
//    }
//}
//
///**********************************************\
//Function Name:  runCommandLine()
//Input Args:     string arg   arguement values
//Output Args:    none
//Description:    Runs Command Line arguements
///**********************************************/
//void Controller::runCommandLine(char *argv[])
//{
//    if(!strcmp(argv[0], "gpioTest"))
//    {
//
//        gpioTest(GPIO_TEST_LENGTH_SEC);
//    }
//    else if (!strcmp(argv[0], "test"))
//    {
//        printf("Test Argument Received!!\n");
//    }
//    else if (!strcmp(argv[0], "ledNopAsmTest"))
//    {
//        ledNopAsmTest(GPIO_TEST_LENGTH_SEC);
//    }
//    else if (!strcmp(argv[0], "gpioLedSpiTest"))
//    {
//        char * arr = new char[18]();
//        ledCreateColorArr(arr, LED_COLOR_CYAN_DIM, LED_COLOR_PURPLE_DIM, LED_COLOR_YELLOW_DIM, LED_COLOR_WHITE_DIM, LED_COLOR_BLUE_DIM, LED_COLOR_GREEN_DIM);
//        gpioLedSpiTest(arr);
//    }
//    else if (!strcmp(argv[0], "ledInitialiseTest"))
//    {
//        ledInitialiseTest();
//    }
//    else if (!strcmp(argv[0], "interruptTest"))
//    {
//        gpioInitializeLib();
//
//        // initialize Distance Sensors
//        rangingInit();
//
//        // Initialize threads ////
//        initInterrupts();
//
//        btbThread btbThread1;
//        btbThread1.start();
//        //usleep(3000);
//        primaryUpdateState(controllerState::TEST_DISTANCE_SENSORS);
//
//        sleep(20);
//    }
//    else if (!strcmp(argv[0], "soundTest1"))
//    {
//        soundTest1();
//    }
//    else if (!strcmp(argv[0], "soundTest2"))
//    {
//        soundTest2();
//    }
//    else if (!strcmp(argv[0], "soundTest3"))
//    {
//        soundTest3();
//    }
//    else if (!strcmp(argv[0], "rangingTestDistanceSensors"))
//    {
//        rangingTestDistanceSensors();
//    }
//    else
//    {
//        printf("Command: %s Failed to run!!!\n", argv[0]);
//    }
//}

