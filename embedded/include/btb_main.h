/****************************** Module Header ******************************\
Module Name:    btb_main.h
Project:        btb
Author:         Grant Maiden
Description:    Header file for btb.main.c
\***************************************************************************/

extern "C" {
#include "VL53L4CD_api.h"
#include "platform.h"
#include "types.h"
#include "examples.h"
}
int example1(Dev_t dev);
void initDistanceSensors();
void initLeds();
void initInterrupts();

