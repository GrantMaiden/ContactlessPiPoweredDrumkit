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

#include "defines.h"
#include "controller.h"
#include "sound.h"

controllerState currentState = RESET;
controllerState nextState = currentState;

sensorValues sens1Values;
sensorValues sens2Values;
sensorValues sens3Values;
sensorValues sens4Values;
sensorValues sens5Values;
sensorValues sens6Values;

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
                controllerHitDetection();
                controllerUpdateDistanceVelocity();
            }
            break;
        case TEST_DISTANCE_SENSORS:
            if(controllerSensorsReady())
            {
                controllerPrintSensorData();
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
    if (detectionValue > HARD_HIT)
        soundTest1(); // TODO: soundPlay(id, HARD_HIT);
    else if (detectionValue > MEDIUM_HIT)
        soundTest1(); // TODO: soundPlay(id, MEDIUM_HIT);
    else if (detectionValue > LIGHT_HIT)
        soundTest1(); // TODO: soundPlay(id, LIGHT_HIT);

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
}

/**********************************************\
Function Name:  controllerHitDetection
Input Args:     none
Output Args:    void
Description:    computes if a hit was detected
/**********************************************/
static void controllerHitDetection()
{
    int detectionValue;

    detectionValue = (sens1Values.currentVelocity + sens1Values.averageVelocity) * SENSOR1_6_DIRECTION;
    if (detectionValue > LIGHT_HIT)
        controllerSendSound(sensorID::SENSOR1, detectionValue);

    detectionValue = (sens2Values.currentVelocity + sens2Values.averageVelocity) * SENSOR2_3_4_5_DIRECTION;
    if (detectionValue > LIGHT_HIT)
        controllerSendSound(sensorID::SENSOR2, detectionValue);

    detectionValue = (sens3Values.currentVelocity + sens3Values.averageVelocity) * SENSOR2_3_4_5_DIRECTION;
    if (detectionValue > LIGHT_HIT)
        controllerSendSound(sensorID::SENSOR3, detectionValue);

    detectionValue = (sens4Values.currentVelocity + sens4Values.averageVelocity) * SENSOR2_3_4_5_DIRECTION;
    if (detectionValue > LIGHT_HIT)
        controllerSendSound(sensorID::SENSOR4, detectionValue);

    detectionValue = (sens5Values.currentVelocity + sens5Values.averageVelocity) * SENSOR2_3_4_5_DIRECTION;
    if (detectionValue > LIGHT_HIT)
        controllerSendSound(sensorID::SENSOR5, detectionValue);

    detectionValue = (sens6Values.currentVelocity + sens6Values.averageVelocity) * SENSOR1_6_DIRECTION;
    if (detectionValue > LIGHT_HIT)
        controllerSendSound(sensorID::SENSOR6, detectionValue);

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
            sens1Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR2:
            sens2Values.rangeStatus = senseValue.rangeStatus;
            sens2Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR3:
            sens3Values.rangeStatus = senseValue.rangeStatus;
            sens3Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR4:
            sens4Values.rangeStatus = senseValue.rangeStatus;
            sens4Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR5:
            sens5Values.rangeStatus = senseValue.rangeStatus;
            sens5Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
        case sensorID::SENSOR6:
            sens6Values.rangeStatus = senseValue.rangeStatus;
            sens6Values.currentDistance_mm = senseValue.currentDistance_mm;
            break;
    }
}

/**********************************************\
Function Name:  controllerSensorsReady
Input Args:     none
Output Args:    bool- True if sensors are ready
Description:    Checks to see if sensor values are ready. if they are, changes status value to not ready.
/**********************************************/
static bool controllerSensorsReady()
{
    bool sensorsReadyBool = true;
    if (sens1Values.rangeStatus != 0)
        sensorsReadyBool = false;
    if (sens2Values.rangeStatus != 0)
        sensorsReadyBool = true; // TODO: CHANG AFTER REWORK
    if (sens3Values.rangeStatus != 0)
        sensorsReadyBool = false;
    if (sens4Values.rangeStatus != 0)
        sensorsReadyBool = false;
    if (sens5Values.rangeStatus != 0)
        sensorsReadyBool = false;
    if (sens6Values.rangeStatus != 0)
        sensorsReadyBool = false;

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
Function Name:  controllerUpdateDistanceVelocity
Input Args:     none
Output Args:    void
Description:    updates the distances and velocity that are stored in each sensorValues object.
/**********************************************/
static void controllerUpdateDistanceVelocity()
{
    sens1Values.lastDistance_mm = sens1Values.currentDistance_mm;

    sens2Values.lastDistance_mm = sens2Values.currentDistance_mm;

    sens3Values.lastDistance_mm = sens3Values.currentDistance_mm;

    sens4Values.lastDistance_mm = sens4Values.currentDistance_mm;

    sens5Values.lastDistance_mm = sens5Values.currentDistance_mm;

    sens6Values.lastDistance_mm = sens6Values.currentDistance_mm;
}

/**********************************************\
Function Name:  controllerPrintSensorData
Input Args:     none
Output Args:    void
Description:    Prints sensor data to console
/**********************************************/
static void controllerPrintSensorData()
{
    printf("%u\t%u\t%u\t%u\t%u\t%u\n",
         sens1Values.currentDistance_mm,
         sens2Values.currentDistance_mm,
         sens3Values.currentDistance_mm,
         sens4Values.currentDistance_mm,
         sens5Values.currentDistance_mm,
         sens6Values.currentDistance_mm);
}

