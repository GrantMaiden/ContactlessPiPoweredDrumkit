/****************************** Module Header ******************************\
Module Name:    led.cpp
Project:        btb
Author:         Grant Maiden
Description:    led control functions and processes
\***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"
#include "gpio_controller.h"
#include "led.h"


/**********************************************\
Function Name:  ledCreateColorArr
Input Args:     outputArr, led1, led2, led3, led4, led5, led6
Output Args:    void
Description:    creates a single color array combined from input arguments. Will fill outputArr memory with combined data. Performs RGB->GRB byteshift
/**********************************************/
void ledCreateColorArr(char* outputArr,unsigned led1, unsigned led2, unsigned led3, unsigned led4, unsigned led5, unsigned led6)
{
    unsigned arr[6] = {led1,led2,led3,led4,led5,led6};

    int j= 0;
    for(int i=0;i<18;i=i+3)
    {
        outputArr[i]    = (arr[j] >> 8) & 0xff; //g
        outputArr[i+1]  = (arr[j] >> 16) & 0xff; //r
        outputArr[i+2]  = (arr[j]) & 0xff; //b
        j = j+1;
        //printf("low @ index: %i\n", i);
        //i = i + 2;
    }

}





