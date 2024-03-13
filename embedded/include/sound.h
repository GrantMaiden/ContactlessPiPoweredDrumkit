/****************************** Module Header ******************************\
Module Name:    sound.h
Project:        btb
Author:         Grant Maiden
Description:    contains sound definitions and headers
\***************************************************************************/
#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdio.h>
#include <string>
#include "CppThread.h"
#include "defines.h"

#define EN_AUDIO

#ifdef EN_AUDIO

//#define MA_NO_ENGINE
//#define MINIAUDIO_IMPLEMENTATION
#define MA_THREADCALL
//typedef unsigned long ma_thread_result;
typedef void* ma_thread_result;
typedef ma_thread_result (MA_THREADCALL * ma_thread_entry_proc)(void* pData);

extern "C"{
#include "miniaudio.h"
}

#define CHANNELS    2               /* Must be stereo for this example. */
#define SAMPLE_RATE 44100

#define TOTAL_INSTRUMENTS               3
#define TOTAL_INSTRUMENTS_SOUND_TEST2   3

#define PATH_SOUND_ENABLED_WAV          "./sounds/sound_enabled.wav"
#define PATH_SOUND_DISABLED_WAV         "./sounds/sound_disabled.wav"

// Top Hat Sounds... Drums 1 and 2
#define TH_LOW_OPEN                     "./sounds/grant_th_open_low.wav"
#define TH_MEDIUM_OPEN                  "./sounds/grant_th_open_medium.wav"
#define TH_LOUD_OPEN                    "./sounds/grant_th_open_loud.wav"
#define TH_LOW_CLOSED                   "./sounds/grant_th_closed_low.wav"
#define TH_MEDIUM_CLOSED                "./sounds/grant_th_closed_medium.wav"
#define TH_LOUD_CLOSED                  "./sounds/grant_th_closed_loud.wav"
#define TH_LOW_FOOT_OPEN                "./sounds/grant_th_foot_open_low.wav"
#define TH_MEDIUM_FOOT_OPEN             "./sounds/grant_th_foot_open_medium.wav"
#define TH_LOUD_FOOT_OPEN               "./sounds/grant_th_foot_open_loud.wav"
#define TH_LOW_FOOT_CLOSED              "./sounds/grant_th_foot_closed_low.wav"
#define TH_MEDIUM_FOOT_CLOSED           "./sounds/grant_th_foot_closed_medium.wav"
#define TH_LOUD_FOOT_CLOSED             "./sounds/grant_th_foot_closed_loud.wav"

#define DRUM3_LOW                       "./sounds/grant_metal_ting_low.wav"
#define DRUM3_MEDIUM                    "./sounds/grant_metal_ting_medium.wav"
#define DRUM3_LOUD                      "./sounds/grant_metal_ting_loud.wav"

#define DRUM4_LOW                       "./sounds/grant_pop_low.wav"
#define DRUM4_MEDIUM                    "./sounds/grant_pop_medium.wav"
#define DRUM4_LOUD                      "./sounds/grant_pop_loud.wav"

#define DRUM5_LOW                       "./sounds/grant_clap_low.wav"
#define DRUM5_MEDIUM                    "./sounds/grant_clap_medium.wav"
#define DRUM5_LOUD                      "./sounds/grant_clap_loud.wav"

#define DRUM6_LOW                       "./sounds/grant_bass_low.wav"
#define DRUM6_MEDIUM                    "./sounds/grant_bass_medium.wav"
#define DRUM6_LOUD                      "./sounds/grant_bass_loud.wav"

using namespace std;
class Sound
{
    public:
        /**
         * Initialize Sound Variables
         **/
        void soundInit();

        /**
         * Tests sound engine
         **/
        static void soundTest1();

        /**
         * Tests sound engine
         **/
        static void soundTest2();

        /**
         * Tests sound engine
         **/
        void soundTest3();

        /**
         * Plays sound at a path. Sound must be preloaded in sound init.
         * @param path - path of wave file to be played
         **/
        void playSound(const char * path);
    private:

        //char * mixArr = new char[TOTAL_INSTRUMENTS]();
        //char * mixArr[] = {"%s %s %s",TH_LOUD_FOOT_CLOSED, TH_LOUD_OPEN, DRUM3_LOUD};

        //static ma_thread_result custom_job_thread(void* pUserData);
        /**
         * Sound Test 2 Helper. Initializes all engine components.
         **/
        static void soundTest2Helper();

        static ma_thread_result MA_THREADCALL custom_job_thread(void* pUserData);


        /**
         * data callback for sound engine
         * @param ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount
         **/
        static inline void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

        static inline ma_result ma_data_source_read_pcm_frames_f32_ex(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead, ma_format dataSourceFormat, ma_uint32 dataSourceChannels);

        static inline ma_result ma_data_source_read_pcm_frames_f32(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead);

        static inline ma_result ma_data_source_read_pcm_frames_and_mix_f32(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead, float volume);

        static inline void data_callback2(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};
#endif

#ifndef EN_AUDIO
using namespace std;
class Sound
{
    public:
        /**
         * Initialize Sound Variables
         **/
        void soundInit();

        /**
         * Tests sound engine
         **/
        static void soundTest1();

        /**
         * Tests sound engine
         **/
        static void soundTest2();

        /**
         * Tests sound engine
         **/
        void soundTest3();
};
#endif

#endif // SOUND_H






