/****************************** Module Header ******************************\
Module Name:    sound.h
Project:        btb
Author:         Grant Maiden
Description:    contains sound definitions and headers
\***************************************************************************/
extern "C"{
#include "miniaudio.h"
}

#define TOTAL_INSTRUMENTS       3
#define TH_LOUD_FOOT_CLOSED     "./sounds/grant_th_foot_closed_loud.wav"
#define TH_LOUD_OPEN            "./sounds/grant_th_open_loud"
#define DRUM3_LOUD              "./sounds/grant_metal_ting_loud.wav"



/**
 * Initialize Sound Variables
 **/
void soundInit();
/**
 * Tests sound engine
 **/
void soundTest1();

/**
 * Tests sound engine
 **/
void soundTest2();

/**
 * Tests sound engine
 **/
void soundTest3();

/**
 * data callback for sound engine
 * @param ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount
 **/
static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

static ma_result ma_data_source_read_pcm_frames_f32_ex(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead, ma_format dataSourceFormat, ma_uint32 dataSourceChannels);

MA_API ma_result ma_data_source_read_pcm_frames_f32(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead);

MA_API ma_result ma_data_source_read_pcm_frames_and_mix_f32(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead, float volume);

void data_callback2(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

