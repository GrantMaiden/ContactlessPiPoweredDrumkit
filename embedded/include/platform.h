/*
 Copyright (c) 2021, STMicroelectronics - All Rights Reserved

 This file : part of VL53L4CD Ultra Lite Driver and : dual licensed, either
 'STMicroelectronics Proprietary license'
 or 'BSD 3-clause "New" or "Revised" License' , at your option.

*******************************************************************************

 'STMicroelectronics Proprietary license'

*******************************************************************************

 License terms: STMicroelectronics Proprietary in accordance with licensing
 terms at www.st.com/sla0081

 STMicroelectronics confidential
 Reproduction and Communication of this document : strictly prohibited unless
 specifically authorized in writing by STMicroelectronics.


*******************************************************************************

 Alternatively, VL53L4CD Ultra Lite Driver may be distributed under the terms of
 'BSD 3-clause "New" or "Revised" License', in which case the following
 provisions apply instead of the ones mentioned above :

*******************************************************************************

 License terms: BSD 3-clause "New" or "Revised" License.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************
*/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#pragma once

#include <stdint.h>
#include <string.h>


    typedef struct {
        int      fd;    	/* File descriptor */
        uint16_t  address;    	/* I2C address of sensor */
    } VL53L4CD_LinuxDev;
    /**
    * VL53L4CD device instance.
    */


    typedef VL53L4CD_LinuxDev* Dev_t;

    /**
     * @brief Error instance.
     */
    typedef uint8_t VL53L4CD_Error;

    /**
     * @brief If the macro below is defined, the device will be programmed to run
     * with I2C Fast Mode Plus (up to 1MHz). Otherwise, default max value is 400kHz.
     */

    //#define VL53L4CD_I2C_FAST_MODE_PLUS

class PlatformVL53l4CD
{

public:
    /**
     * @brief Init communication
     */

    uint8_t VL53L4CD_comms_init(Dev_t dev);

    /**
     * @brief Close communication
     */

    uint8_t VL53L4CD_comms_close(Dev_t dev);

    /**
     * @brief Read 32 bits through I2C.
     */

    uint8_t VL53L4CD_RdDWord(Dev_t dev, uint16_t registerAddr, uint32_t *value);
    /**
     * @brief Read 16 bits through I2C.
     */

    uint8_t VL53L4CD_RdWord(Dev_t dev, uint16_t registerAddr, uint16_t *value);

    /**
     * @brief Read 8 bits through I2C.
     */

    uint8_t VL53L4CD_RdByte(Dev_t dev, uint16_t registerAddr, uint8_t *value);

    /**
     * @brief Write 8 bits through I2C.
     */

    uint8_t VL53L4CD_WrByte(Dev_t dev, uint16_t registerAddr, uint8_t value);

    /**
     * @brief Write 16 bits through I2C.
     */

    uint8_t VL53L4CD_WrWord(Dev_t dev, uint16_t RegisterAdress, uint16_t value);

    /**
     * @brief Write 32 bits through I2C.
     */

    uint8_t VL53L4CD_WrDWord(Dev_t dev, uint16_t RegisterAdress, uint32_t value);

    /**
     * @brief Wait during N milliseconds.
     */

    uint8_t WaitMs(Dev_t dev, uint32_t TimeMs);

private:
    uint8_t write_read_multi(
		Dev_t dev,
		uint16_t reg_address,
		uint8_t *pdata,
		uint32_t count,
		int write_not_read);

    uint8_t write_multi(
		Dev_t dev,
		uint16_t reg_address,
		uint8_t *pdata,
		uint32_t count);

    uint8_t read_multi(
		Dev_t dev,
		uint16_t reg_address,
		uint8_t *pdata,
		uint32_t count);

    uint8_t VL53L4CD_ReadMulti(
        Dev_t dev,
        uint16_t RegisterAdress,
        uint8_t *p_values,
        uint32_t size);

    #endif	// _PLATFORM_H_
};
