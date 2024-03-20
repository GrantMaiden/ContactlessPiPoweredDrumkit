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

#define PATH_SOUND_ENABLED_WAV          "./sounds/GRANT_KIT/sound_enabled.wav"
#define PATH_SOUND_DISABLED_WAV         "./sounds/GRANT_KIT/sound_disabled.wav"

// Top Hat Sounds/GRANT_KIT... Drums 1 and 2
#define TH_LOW_OPEN_GRANT                    "./sounds/GRANT_KIT/grant_th_open_loud.wav"
#define TH_MEDIUM_OPEN_GRANT                  "./sounds/GRANT_KIT/grant_th_open_medium.wav"
#define TH_LOUD_OPEN_GRANT                    "./sounds/GRANT_KIT/grant_th_open_loud.wav"
#define TH_LOW_CLOSED_GRANT                   "./sounds/GRANT_KIT/grant_th_closed_low.wav"
#define TH_MEDIUM_CLOSED_GRANT                "./sounds/GRANT_KIT/grant_th_closed_medium.wav"
#define TH_LOUD_CLOSED_GRANT                  "./sounds/GRANT_KIT/grant_th_closed_loud.wav"
#define TH_LOW_FOOT_OPEN_GRANT                "./sounds/GRANT_KIT/grant_th_foot_open_low.wav"
#define TH_MEDIUM_FOOT_OPEN_GRANT             "./sounds/GRANT_KIT/grant_th_foot_open_medium.wav"
#define TH_LOUD_FOOT_OPEN_GRANT               "./sounds/GRANT_KIT/grant_th_foot_open_loud.wav"
#define TH_LOW_FOOT_CLOSED_GRANT              "./sounds/GRANT_KIT/grant_th_foot_closed_low.wav"
#define TH_MEDIUM_FOOT_CLOSED_GRANT           "./sounds/GRANT_KIT/grant_th_foot_closed_medium.wav"
#define TH_LOUD_FOOT_CLOSED_GRANT             "./sounds/GRANT_KIT/grant_th_foot_closed_loud.wav"

#define DRUM3_LOW_GRANT                       "./sounds/GRANT_KIT/grant_metal_ting_low.wav"
#define DRUM3_MEDIUM_GRANT                    "./sounds/GRANT_KIT/grant_metal_ting_medium.wav"
#define DRUM3_LOUD_GRANT                      "./sounds/GRANT_KIT/grant_metal_ting_loud.wav"

#define DRUM4_LOW_GRANT                       "./sounds/GRANT_KIT/grant_pop_low.wav"
#define DRUM4_MEDIUM_GRANT                    "./sounds/GRANT_KIT/grant_pop_medium.wav"
#define DRUM4_LOUD_GRANT                      "./sounds/GRANT_KIT/grant_pop_loud.wav"

#define DRUM5_LOW_GRANT                       "./sounds/GRANT_KIT/grant_clap_low.wav"
#define DRUM5_MEDIUM_GRANT                    "./sounds/GRANT_KIT/grant_clap_medium.wav"
#define DRUM5_LOUD_GRANT                      "./sounds/GRANT_KIT/grant_clap_loud.wav"

#define DRUM6_LOW_GRANT                       "./sounds/GRANT_KIT/grant_bass_low.wav"
#define DRUM6_MEDIUM_GRANT                    "./sounds/GRANT_KIT/grant_bass_medium.wav"
#define DRUM6_LOUD_GRANT                      "./sounds/GRANT_KIT/grant_bass_loud.wav"


// Drumkit sounds for Byte The Beat//


// Drum Sounds (Snare, Hi Tom, Mid Tom, Low Tom, Kick//
#define SNARE_SOFT                             "./sounds/PI_KIT_SOUNDS/Snare/Center/Snare Center Soft.wav"
#define SNARE_MID_SOFT                         "./sounds/PI_KIT_SOUNDS/Snare/Center/Snare Center Mid-Soft.wav"
#define SNARE_MID                              "./sounds/PI_KIT_SOUNDS/Snare/Center/Snare Center Mid.wav"
#define SNARE_MID_HARD                         "./sounds/PI_KIT_SOUNDS/Snare/Center/Snare Center Mid-Hard.wav"
#define SNARE_HARD                             "./sounds/PI_KIT_SOUNDS/Snare/Center/Snare Center Hard.wav"

#define HI_TOM_SOFT                            "./sounds/PI_KIT_SOUNDS/Hi Tom/Hi Tom Soft.wav"
#define HI_TOM_MID_SOFT                        "./sounds/PI_KIT_SOUNDS/Hi Tom/Hi Tom Mid-Soft.wav"
#define HI_TOM_MID                             "./sounds/PI_KIT_SOUNDS/Hi Tom/Hi Tom Mid.wav"
#define HI_TOM_MID_HARD                        "./sounds/PI_KIT_SOUNDS/Hi Tom/Hi Tom Mid-Hard.wav"
#define HI_TOM_HARD                            "./sounds/PI_KIT_SOUNDS/Hi Tom/Hi Tom Hard.wav"

#define MID_TOM_SOFT                           "./sounds/PI_KIT_SOUNDS/Mid Tom/Mid Tom Soft.wav"
#define MID_TOM_MID_SOFT                       "./sounds/PI_KIT_SOUNDS/Mid Tom/Mid Tom Mid-Soft.wav"
#define MID_TOM_MID                            "./sounds/PI_KIT_SOUNDS/Mid Tom/Mid Tom Mid.wav"
#define MID_TOM_MID_HARD                       "./sounds/PI_KIT_SOUNDS/Mid Tom/Mid Tom Mid-Hard.wav"
#define MID_TOM_HARD                           "./sounds/PI_KIT_SOUNDS/Mid Tom/Mid Tom Hard.wav"

#define LOW_TOM_SOFT                           "./sounds/PI_KIT_SOUNDS/Low Tom/Low Tom Soft.wav" 
#define LOW_TOM_MID_SOFT                       "./sounds/PI_KIT_SOUNDS/Low Tom/Low Tom Mid-Soft.wav"
#define LOW_TOM_MID                            "./sounds/PI_KIT_SOUNDS/Low Tom/Low Tom Mid.wav"
#define LOW_TOM_MID_HARD                       "./sounds/PI_KIT_SOUNDS/Low Tom/Low Tom Mid-Hard.wav"
#define LOW_TOM_HARD                           "./sounds/PI_KIT_SOUNDS/Low Tom/Low Tom Hard.wav"

#define KICK_SOFT                              "./sounds/PI_KIT_SOUNDS/Kick/Kick Soft.wav"  
#define KICK_MID_SOFT                          "./sounds/PI_KIT_SOUNDS/Kick/Kick Mid-Soft.wav" 
#define KICK_MID                               "./sounds/PI_KIT_SOUNDS/Kick/Kick Mid.wav" 
#define KICK_MID_HARD                          "./sounds/PI_KIT_SOUNDS/Kick/Kick Mid-Hard.wav" 
#define KICK_HARD                              "./sounds/PI_KIT_SOUNDS/Kick/Kick Hard.wav" 

//Cymbal Sounds//

//Hi Hat//
#define HI_HAT_OPEN_SOFT                        "./sounds/PI_KIT_SOUNDS/Hi Hat/Open/Hi-Hat Open Soft.wav" 
#define HI_HAT_OPEN_MID_SOFT                    "./sounds/PI_KIT_SOUNDS/Hi Hat/Open/Hi-Hat Open Mid-Soft.wav"
#define HI_HAT_OPEN_MID                         "./sounds/PI_KIT_SOUNDS/Hi Hat/Open/Hi-Hat Open Mid.wav"
#define HI_HAT_OPEN_MID_HARD                    "./sounds/PI_KIT_SOUNDS/Hi Hat/Open/Hi-Hat Open Mid-Hard.wav"
#define HI_HAT_OPEN_HARD                        "./sounds/PI_KIT_SOUNDS/Hi Hat/Open/Hi-Hat Open Hard.wav"

#define HI_HAT_CLOSED_SOFT                      "./sounds/PI_KIT_SOUNDS/Hi Hat/Closed/Hi-Hat Closed Soft.wav" 
#define HI_HAT_CLOSED_MID_SOFT                  "./sounds/PI_KIT_SOUNDS/Hi Hat/Closed/Hi-Hat Closed Mid-Soft.wav"
#define HI_HAT_CLOSED_MID                       "./sounds/PI_KIT_SOUNDS/Hi Hat/Closed/Hi-Hat Closed Mid.wav"
#define HI_HAT_CLOSED_MID_HARD                  "./sounds/PI_KIT_SOUNDS/Hi Hat/Closed/Hi-Hat Closed Mid-Hard.wav"
#define HI_HAT_CLOSED_HARD                      "./sounds/PI_KIT_SOUNDS/Hi Hat/Closed/Hi-Hat Closed Hard.wav"

#define HI_HAT_FOOT_CLOSED_SOFT                 "./sounds/PI_KIT_SOUNDS/Hi Hat/Foot/Hi-Hat Foot Soft.wav" 
#define HI_HAT_FOOT_CLOSED_MID_SOFT             "./sounds/PI_KIT_SOUNDS/Hi Hat/Foot/Hi-Hat Foot Mid-Soft.wav"
#define HI_HAT_FOOT_CLOSED_MID                  "./sounds/PI_KIT_SOUNDS/Hi Hat/Foot/Hi-Hat Foot Mid.wav"
#define HI_HAT_FOOT_CLOSED_MID_HARD             "./sounds/PI_KIT_SOUNDS/Hi Hat/Foot/Hi-Hat Foot Mid-Hard.wav"
#define HI_HAT_FOOT_CLOSED_HARD                 "./sounds/PI_KIT_SOUNDS/Hi Hat/Foot/Hi-Hat Foot Hard.wav"

#define CRASH_SOFT                              "./sounds/PI_KIT_SOUNDS/Crash/Crash Soft.wav" 
#define CRASH_MID_SOFT                          "./sounds/PI_KIT_SOUNDS/Crash/Crash Mid-Soft.wav"
#define CRASH_MID                               "./sounds/PI_KIT_SOUNDS/Crash/Crash Mid.wav"
#define CRASH_MID_HARD                          "./sounds/PI_KIT_SOUNDS/Crash/Crash Mid-Hard.wav"
#define CRASH_HARD                              "./sounds/PI_KIT_SOUNDS/Crash/Crash Hard.wav"

#define RIDE_SOFT                               "./sounds/PI_KIT_SOUNDS/Ride/Ride Soft.wav" 
#define RIDE_MID_SOFT                           "./sounds/PI_KIT_SOUNDS/Ride/Ride Mid-Soft.wav"
#define RIDE_MID                                "./sounds/PI_KIT_SOUNDS/Ride/Ride Mid.wav"
#define RIDE_MID_HARD                           "./sounds/PI_KIT_SOUNDS/Ride/Ride Mid-Hard.wav"
#define RIDE_HARD                               "./sounds/PI_KIT_SOUNDS/Ride/Ride Hard.wav"

//AUX Percussion Sounds//
#define CLAP_SOFT                               "./sounds/PI_KIT_SOUNDS/Clap/Clap Soft.wav" 
#define CLAP_MID_SOFT                           "./sounds/PI_KIT_SOUNDS/Clap/Clap Mid-Soft.wav"
#define CLAP_MID                                "./sounds/PI_KIT_SOUNDS/Clap/Clap Mid.wav"
#define CLAP_MID_HARD                           "./sounds/PI_KIT_SOUNDS/Clap/Clap Mid-Hard.wav"
#define CLAP_HARD                               "./sounds/PI_KIT_SOUNDS/Clap/Clap Hard.wav"

#define BELL_SOFT                               "./sounds/PI_KIT_SOUNDS/Bell/Bell Soft.wav" 
#define BELL_MID_SOFT                           "./sounds/PI_KIT_SOUNDS/Bell/Bell Mid-Soft.wav"
#define BELL_MID                                "./sounds/PI_KIT_SOUNDS/Bell/Bell Mid.wav"
#define BELL_MID_HARD                           "./sounds/PI_KIT_SOUNDS/Bell/Bell Mid-Hard.wav"
#define BELL_HARD                               "./sounds/PI_KIT_SOUNDS/Bell/Bell Hard.wav"

#define COWBELL_SOFT                            "./sounds/PI_KIT_SOUNDS/Cowbell/Cowbell Soft.wav" 
#define COWBELL_MID_SOFT                        "./sounds/PI_KIT_SOUNDS/Cowbell/Cowbell Mid-Soft.wav"
#define COWBELL_MID                             "./sounds/PI_KIT_SOUNDS/Cowbell/Cowbell Mid.wav"
#define COWBELL_MID_HARD                        "./sounds/PI_KIT_SOUNDS/Cowbell/Cowbell Mid-Hard.wav"
#define COWBELL_HARD                            "./sounds/PI_KIT_SOUNDS/Cowbell/Cowbell Hard.wav"


//Sound class//
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






