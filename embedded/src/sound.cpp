/****************************** Module Header ******************************\
Module Name:    sound.cpp
Project:        btb
Author:         Grant Maiden
Description:    sound control functions and processes
\***************************************************************************/
/*
This file is developed using resources and exmaples available as part of the
miniAudio library. These examples are available on the corresponding github
page for the miniAudio library. For full transparancy, the miniaudio license
is displayed below, in full.
*/

/*
This software is available as a choice of the following licenses. Choose
whichever you prefer.

===============================================================================
ALTERNATIVE 1 - Public Domain (www.unlicense.org)
===============================================================================
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

===============================================================================
ALTERNATIVE 2 - MIT No Attribution
===============================================================================
Copyright 2023 David Reid

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <time.h>

#define MINIAUDIO_IMPLEMENTATION
extern "C"{
#include "miniaudio.h"
}
#include "sound.h"

//#ifdef EN_AUDIO
//#define MA_NO_ENGINE
//#define MINIAUDIO_IMPLEMENTATION
//extern "C"{
//#include "miniaudio.h"
//}
//#endif

#define EN_AUDIO

#ifdef EN_AUDIO
//#define MA_NO_ENGINE
//#define MINIAUDIO_IMPLEMENTATION
//extern "C"{
//#include "miniaudio.h"
//}



//static char * mixArr[TOTAL_INSTRUMENTS];
char * mixArr[] = {"%s %s %s",TH_LOUD_FOOT_CLOSED, TH_LOUD_OPEN, DRUM3_LOUD};

static ma_resource_manager_data_source g_dataSources[16];
static ma_uint32                       g_dataSourceCount;

ma_engine audio_engine;
ma_engine_config engineConfig;
ma_sound * pPreloadedSounds;

/**********************************************\
Function Name:  soundInit
Input Args:     none
Output Args:    void
Description:    initialize sound variables
/**********************************************/
void Sound::soundInit()
{
    ma_result result;

    result = ma_engine_init(NULL, &audio_engine);
    if (result != MA_SUCCESS) {
        printf("Error initing engine...");
    }
    // Sound Enabled Initialised//
    ma_sound_init_from_file(&audio_engine, PATH_SOUND_ENABLED_WAV, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, PATH_SOUND_DISABLED_WAV, 0, NULL, NULL, &pPreloadedSounds[0]);

    // Grant Kit Initialised//
    ma_sound_init_from_file(&audio_engine, TH_LOW_OPEN, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_MEDIUM_OPEN, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_LOUD_OPEN, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_LOW_CLOSED, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_MEDIUM_CLOSED, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_LOUD_CLOSED, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_LOW_FOOT_OPEN, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_MEDIUM_FOOT_OPEN, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_LOUD_FOOT_OPEN, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_LOW_FOOT_CLOSED, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_MEDIUM_FOOT_CLOSED, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, TH_LOUD_FOOT_CLOSED, 0, NULL, NULL, &pPreloadedSounds[0]);

    ma_sound_init_from_file(&audio_engine, DRUM3_LOW, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM3_MEDIUM, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM3_LOUD, 0, NULL, NULL, &pPreloadedSounds[0]);

    ma_sound_init_from_file(&audio_engine, DRUM4_LOW, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM4_MEDIUM, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM4_LOUD, 0, NULL, NULL, &pPreloadedSounds[0]);

    ma_sound_init_from_file(&audio_engine, DRUM5_LOW, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM5_MEDIUM, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM5_LOUD, 0, NULL, NULL, &pPreloadedSounds[0]);

    ma_sound_init_from_file(&audio_engine, DRUM6_LOW, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM6_MEDIUM, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, DRUM6_LOUD, 0, NULL, NULL, &pPreloadedSounds[0]);

    // PI Kit Initialised//   

    //Drum Sounds//

    //Snare Sound
    ma_sound_init_from_file(&audio_engine, SNARE_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, SNARE_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, SNARE_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, SNARE_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, SNARE_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Hi Tom Sound
    ma_sound_init_from_file(&audio_engine, HI_TOM_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_TOM_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_TOM_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_TOM_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_TOM_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Mid Tom Sound
    ma_sound_init_from_file(&audio_engine, MID_TOM_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, MID_TOM_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, MID_TOM_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, MID_TOM_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, MID_TOM_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Low Tom Sound
    ma_sound_init_from_file(&audio_engine, LOW_TOM_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, LOW_TOM_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, LOW_TOM_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, LOW_TOM_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, LOW_TOM_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Kick Sound
    ma_sound_init_from_file(&audio_engine, KICK_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, KICK_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, KICK_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, KICK_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, KICK_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Cymbal Sounds//

    //Hi Hat Sounds//
    //Hi Hat Open
    ma_sound_init_from_file(&audio_engine, HI_HAT_OPEN_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_OPEN_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_OPEN_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_OPEN_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_OPEN_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Hi Hat Closed
    ma_sound_init_from_file(&audio_engine, HI_HAT_CLOSED_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_CLOSED_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_CLOSED_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_CLOSED_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_CLOSED_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Hi Hat Foot
    ma_sound_init_from_file(&audio_engine, HI_HAT_FOOT_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_FOOT_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_FOOT_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_FOOT_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, HI_HAT_FOOT_SOFT 0, NULL, NULL, &pPreloadedSounds[0]);

    //Other Cymbals//
    //Crash Sound
    ma_sound_init_from_file(&audio_engine, CRASH_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CRASH_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CRASH_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CRASH_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CRASH_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Ride Sound
    ma_sound_init_from_file(&audio_engine, RIDE_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, RIDE_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, RIDE_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, RIDE_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, RIDE_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //AUX Percussion Sounds//
    //Clap Sound
    ma_sound_init_from_file(&audio_engine, CLAP_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CLAP_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CLAP_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CLAP_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, CLAP_SOFT 0, NULL, NULL, &pPreloadedSounds[0]);

    //Bell Sound
    ma_sound_init_from_file(&audio_engine, BELL_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, BELL_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, BELL_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, BELL_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, BELL_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);

    //Cowbell Sound
    ma_sound_init_from_file(&audio_engine, COWBELL_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, COWBELL_MID_HARD, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, COWBELL_MID, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, COWBELL_MID_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
    ma_sound_init_from_file(&audio_engine, COWBELL_SOFT, 0, NULL, NULL, &pPreloadedSounds[0]);
}


/**********************************************\
Function Name:  playSound
Input Args:     path - path of wave file to be played
Output Args:    void
Description:    Plays sound at a path. Sound must be preloaded in sound init.
/**********************************************/
void Sound::playSound(const char * path)
{
    ma_engine_play_sound(&audio_engine, path, NULL);
}

/**********************************************\
Function Name:  soundTest1
Input Args:     none
Output Args:    void
Description:    test Sound engine
/**********************************************/
void Sound::soundTest1()
{
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file(TH_LOUD_FOOT_CLOSED, NULL, &decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file: %s\n", TH_LOUD_FOOT_CLOSED);
        return;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return;
    }

    sleep(1);

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}

/**********************************************\
Function Name:  data_callback
Input Args:     ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount
Output Args:    void
Description:    data callback for sound engine
/**********************************************/
void Sound::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

/**********************************************\
Function Name:  soundTest2
Input Args:     none
Output Args:    void
Description:    test Sound engine
/**********************************************/
void Sound::soundTest2()
{

    ma_result result;
    ma_device_config deviceConfig;
    ma_device device;
    ma_resource_manager_config resourceManagerConfig;
    ma_resource_manager resourceManager;
    ma_thread jobThread;
    int iFile;

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_f32;
    deviceConfig.dataCallback    = data_callback2;
    deviceConfig.pUserData       = NULL;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize device.");
        return;
    }


    /* We can start the device before loading any sounds. We'll just end up outputting silence. */
    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.");
        return;
    }


    /*
    We have the device so now we want to initialize the resource manager. We'll use the resource manager to load some
    sounds based on the command line.
    */
    resourceManagerConfig = ma_resource_manager_config_init();

    /*
    We'll set a standard decoding format to save us to processing time at mixing time. If you're wanting to use
    spatialization with your decoded sounds, you may want to consider leaving this as 0 to ensure the file's native
    channel count is used so you can do proper spatialization.
    */
    resourceManagerConfig.decodedFormat     = device.playback.format;
    resourceManagerConfig.decodedChannels   = device.playback.channels;
    resourceManagerConfig.decodedSampleRate = device.sampleRate;

    /* The number of job threads to be managed internally. Set this to 0 if you want to self-manage your job threads */
    resourceManagerConfig.jobThreadCount = 4;

    result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to initialize the resource manager.");
        return;
    }

    /*
    Now that we have a resource manager we can set up our custom job thread. This is optional. Normally when doing
    self-managed job threads you would set the internal job thread count to zero. We're doing both internal and
    self-managed job threads in this example just for demonstration purposes.
    */
    ma_thread_create(&jobThread, ma_thread_priority_default, 0, custom_job_thread, &resourceManager, NULL);

    /* Create each data source from the resource manager. Note that the caller is the owner. */
    for (iFile = 0; iFile < ma_countof(g_dataSources) && iFile < TOTAL_INSTRUMENTS_SOUND_TEST2; iFile += 1) {
        result = ma_resource_manager_data_source_init(
            &resourceManager,
            mixArr[iFile+1],
            MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC /*| MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM*/,
            NULL,   /* Async notification. */
            &g_dataSources[iFile]);

        if (result != MA_SUCCESS) {
            break;
        }

        /* Use looping in this example. */
        ma_data_source_set_looping(&g_dataSources[iFile], MA_TRUE);

        g_dataSourceCount += 1;
    }

    printf("Press Enter to quit...");
    getchar();


    /* Teardown. */

    /*
    Uninitialize the device first to ensure the data callback is stopped and doesn't try to access
    any data.
    */
    ma_device_uninit(&device);

    /*
    Our data sources need to be explicitly uninitialized. ma_resource_manager_uninit() will not do
    it for us. This needs to be done before posting the quit event and uninitializing the resource
    manager or else we'll get stuck in a deadlock because ma_resource_manager_data_source_uninit()
    will be waiting for the job thread(s) to finish work, which will never happen because they were
    just terminated.
    */
    for (iFile = 0; (size_t)iFile < g_dataSourceCount; iFile += 1) {
        ma_resource_manager_data_source_uninit(&g_dataSources[iFile]);
    }

    /*
    Before uninitializing the resource manager we need to make sure a quit event has been posted to
    ensure we can get out of our custom thread. The call to ma_resource_manager_uninit() will also
    do this, but we need to call it explicitly so that our self-managed thread can exit naturally.
    You only need to post a quit job if you're using that as the exit indicator. You can instead
    use whatever variable you want to terminate your job thread, but since this example is using a
    quit job we need to post one. Note that you don't need to do this if you're not managing your
    own threads - ma_resource_manager_uninit() alone will suffice in that case.
    */
    ma_resource_manager_post_job_quit(&resourceManager);
    ma_thread_wait(&jobThread); /* Wait for the custom job thread to finish so it doesn't try to access any data. */

    /* Uninitialize the resource manager after each data source. */
    ma_resource_manager_uninit(&resourceManager);

    return;

}
/**********************************************\
Function Name:  soundTest2
Input Args:     none
Output Args:    void
Description:    test Sound engine
/**********************************************/
void Sound::soundTest2Helper()
{
    ma_result result;
    ma_device_config deviceConfig;
    ma_device device;
    ma_resource_manager_config resourceManagerConfig;
    ma_resource_manager resourceManager;
    ma_thread jobThread;
    int iFile;

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_f32;
    deviceConfig.dataCallback    = data_callback2;
    deviceConfig.pUserData       = NULL;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize device.");
        return;
    }


    /* We can start the device before loading any sounds. We'll just end up outputting silence. */
    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.");
        return;
    }


    /*
    We have the device so now we want to initialize the resource manager. We'll use the resource manager to load some
    sounds based on the command line.
    */
    resourceManagerConfig = ma_resource_manager_config_init();

    /*
    We'll set a standard decoding format to save us to processing time at mixing time. If you're wanting to use
    spatialization with your decoded sounds, you may want to consider leaving this as 0 to ensure the file's native
    channel count is used so you can do proper spatialization.
    */
    resourceManagerConfig.decodedFormat     = device.playback.format;
    resourceManagerConfig.decodedChannels   = device.playback.channels;
    resourceManagerConfig.decodedSampleRate = device.sampleRate;

    /* The number of job threads to be managed internally. Set this to 0 if you want to self-manage your job threads */
    resourceManagerConfig.jobThreadCount = 4;

    result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to initialize the resource manager.");
        return;
    }

    /*
    Now that we have a resource manager we can set up our custom job thread. This is optional. Normally when doing
    self-managed job threads you would set the internal job thread count to zero. We're doing both internal and
    self-managed job threads in this example just for demonstration purposes.
    */
    ma_thread_create(&jobThread, ma_thread_priority_default, 0, custom_job_thread, &resourceManager, NULL);

    /* Create each data source from the resource manager. Note that the caller is the owner. */
    for (iFile = 0; iFile < ma_countof(g_dataSources) && iFile < TOTAL_INSTRUMENTS_SOUND_TEST2; iFile += 1) {
        result = ma_resource_manager_data_source_init(
            &resourceManager,
            mixArr[iFile+1],
            MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC /*| MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM*/,
            NULL,   /* Async notification. */
            &g_dataSources[iFile]);

        if (result != MA_SUCCESS) {
            break;
        }

        /* Use looping in this example. */
        ma_data_source_set_looping(&g_dataSources[iFile], 0);

        g_dataSourceCount += 1;
    }

    //printf("Press Enter to quit...");
    //getchar();



    /* Teardown. */

    /*
    Uninitialize the device first to ensure the data callback is stopped and doesn't try to access
    any data.
    */
    ma_device_uninit(&device);

    /*
    Our data sources need to be explicitly uninitialized. ma_resource_manager_uninit() will not do
    it for us. This needs to be done before posting the quit event and uninitializing the resource
    manager or else we'll get stuck in a deadlock because ma_resource_manager_data_source_uninit()
    will be waiting for the job thread(s) to finish work, which will never happen because they were
    just terminated.
    */
    for (iFile = 0; (size_t)iFile < g_dataSourceCount; iFile += 1) {
        ma_resource_manager_data_source_uninit(&g_dataSources[iFile]);
    }

    /*
    Before uninitializing the resource manager we need to make sure a quit event has been posted to
    ensure we can get out of our custom thread. The call to ma_resource_manager_uninit() will also
    do this, but we need to call it explicitly so that our self-managed thread can exit naturally.
    You only need to post a quit job if you're using that as the exit indicator. You can instead
    use whatever variable you want to terminate your job thread, but since this example is using a
    quit job we need to post one. Note that you don't need to do this if you're not managing your
    own threads - ma_resource_manager_uninit() alone will suffice in that case.
    */
    ma_resource_manager_post_job_quit(&resourceManager);
    ma_thread_wait(&jobThread); /* Wait for the custom job thread to finish so it doesn't try to access any data. */

    /* Uninitialize the resource manager after each data source. */
    ma_resource_manager_uninit(&resourceManager);

    return;
}

/**********************************************\
Function Name:  soundTest3
Input Args:     none
Output Args:    void
Description:    test Sound engine
/**********************************************/
void Sound::soundTest3()
{
    soundInit();

    for (int i = 0; i <= 10; i++)
    {
        playSound(TH_LOUD_FOOT_CLOSED);
        playSound(TH_LOUD_OPEN);
        playSound(DRUM3_LOUD);
        usleep(200000);
    }



    printf("Press Enter to quit...");
    getchar();

    ma_sound_uninit(&pPreloadedSounds[0]);
    ma_engine_uninit(&audio_engine);
}

ma_result Sound::ma_data_source_read_pcm_frames_f32_ex(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead, ma_format dataSourceFormat, ma_uint32 dataSourceChannels)
{
    /*
    This function is intended to be used when the format and channel count of the data source is
    known beforehand. The idea is to avoid overhead due to redundant calls to ma_data_source_get_data_format().
    */
    MA_ASSERT(pDataSource != NULL);

    if (dataSourceFormat == ma_format_f32) {
        /* Fast path. No conversion necessary. */
        return ma_data_source_read_pcm_frames(pDataSource, pFramesOut, frameCount, pFramesRead);
    } else {
        /* Slow path. Conversion necessary. */
        ma_result result;
        ma_uint64 totalFramesRead;
        ma_uint8 temp[MA_DATA_CONVERTER_STACK_BUFFER_SIZE];
        ma_uint64 tempCapInFrames = sizeof(temp) / ma_get_bytes_per_frame(dataSourceFormat, dataSourceChannels);

        totalFramesRead = 0;
        while (totalFramesRead < frameCount) {
            ma_uint64 framesJustRead;
            ma_uint64 framesToRead = frameCount - totalFramesRead;
            if (framesToRead > tempCapInFrames) {
                framesToRead = tempCapInFrames;
            }

            result = ma_data_source_read_pcm_frames(pDataSource, pFramesOut, framesToRead, &framesJustRead);

            ma_convert_pcm_frames_format(ma_offset_pcm_frames_ptr_f32(pFramesOut, totalFramesRead, dataSourceChannels), ma_format_f32, temp, dataSourceFormat, framesJustRead, dataSourceChannels, ma_dither_mode_none);
            totalFramesRead += framesJustRead;

            if (result != MA_SUCCESS) {
                break;
            }
        }

        return MA_SUCCESS;
    }
}

ma_result Sound::ma_data_source_read_pcm_frames_f32(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead)
{
    ma_result result;
    ma_format format;
    ma_uint32 channels;

    result = ma_data_source_get_data_format(pDataSource, &format, &channels, NULL, NULL, 0);
    if (result != MA_SUCCESS) {
        return result;  /* Failed to retrieve the data format of the data source. */
    }

    return ma_data_source_read_pcm_frames_f32_ex(pDataSource, pFramesOut, frameCount, pFramesRead, format, channels);
}

ma_result Sound::ma_data_source_read_pcm_frames_and_mix_f32(ma_data_source* pDataSource, float* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead, float volume)
{
    ma_result result;
    ma_format format;
    ma_uint32 channels;
    ma_uint64 totalFramesRead;

    if (pFramesRead != NULL) {
        *pFramesRead = 0;
    }

    if (pDataSource == NULL) {
        return MA_INVALID_ARGS;
    }

    result = ma_data_source_get_data_format(pDataSource, &format, &channels, NULL, NULL, 0);
    if (result != MA_SUCCESS) {
        return result;  /* Failed to retrieve the data format of the data source. */
    }

    totalFramesRead = 0;
    while (totalFramesRead < frameCount) {
        float temp[MA_DATA_CONVERTER_STACK_BUFFER_SIZE/sizeof(float)];
        ma_uint64 tempCapInFrames = ma_countof(temp) / channels;
        ma_uint64 framesJustRead;
        ma_uint64 framesToRead = frameCount - totalFramesRead;
        if (framesToRead > tempCapInFrames) {
            framesToRead = tempCapInFrames;
        }

        result = ma_data_source_read_pcm_frames_f32_ex(pDataSource, temp, framesToRead, &framesJustRead, format, channels);

        ma_mix_pcm_frames_f32((float*)ma_offset_pcm_frames_ptr(pFramesOut, totalFramesRead, ma_format_f32, channels), temp, framesJustRead, channels, volume);
        totalFramesRead += framesJustRead;

        if (result != MA_SUCCESS) {
            break;
        }
    }

    if (pFramesRead != NULL) {
        *pFramesRead = totalFramesRead;
    }

    return MA_SUCCESS;
}

void Sound::data_callback2(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    /*
    In this example we're just going to play our data sources layered on top of each other. This
    assumes the device's format is f32 and that the buffer is not pre-silenced.
    */
    ma_uint32 iDataSource;

    MA_ASSERT(pDevice->playback.format == ma_format_f32);

    (void)pInput;   /* Unused. */

    /*
    If the device was configured with noPreSilencedOutputBuffer then you would need to silence the
    buffer here, or make sure the first data source to be mixed is copied rather than mixed.
    */
    /*ma_silence_pcm_frames(pOutput, frameCount, ma_format_f32, pDevice->playback.channels);*/

    /* For each sound, mix as much data as we can. */
    for (iDataSource = 0; iDataSource < g_dataSourceCount; iDataSource += 1) {
        ma_data_source_read_pcm_frames_and_mix_f32(&g_dataSources[iDataSource], (float*)pOutput, frameCount, NULL, /* volume = */1);
    }
}


ma_thread_result MA_THREADCALL Sound::custom_job_thread(void* pUserData)
{
    ma_resource_manager* pResourceManager = (ma_resource_manager*)pUserData;
    MA_ASSERT(pResourceManager != NULL);

    for (;;) {
        ma_result result;
        ma_resource_manager_job job;

        /*
        Retrieve a job from the queue first. This defines what it is you're about to do. By default this will be
        blocking. You can initialize the resource manager with MA_RESOURCE_MANAGER_FLAG_NON_BLOCKING to not block in
        which case MA_NO_DATA_AVAILABLE will be returned if no jobs are available.

        When the quit job is returned (MA_RESOURCE_MANAGER_JOB_QUIT), the return value will always be MA_CANCELLED. If you don't want
        to check the return value (you should), you can instead check if the job code is MA_RESOURCE_MANAGER_JOB_QUIT and use that
        instead.
        */
        result = ma_resource_manager_next_job(pResourceManager, &job);
        if (result != MA_SUCCESS) {
            if (result == MA_CANCELLED) {
                printf("CUSTOM JOB THREAD TERMINATING VIA MA_CANCELLED... ");
            } else {
                printf("CUSTOM JOB THREAD ERROR: %s. TERMINATING... ", ma_result_description(result));
            }

            break;
        }

        /*
        Terminate if we got a quit message. You don't need to terminate like this, but's a bit more robust. You can
        just use a global variable or something similar if it's easier for your particular situation. The quit job
        remains in the queue and will continue to be returned by future calls to ma_resource_manager_next_job(). The
        reason for this is to give every job thread visibility to the quit job so they have a chance to exit.

        We won't actually be hitting this code because the call above will return MA_CANCELLED when the MA_RESOURCE_MANAGER_JOB_QUIT
        event is received which means the `result != MA_SUCCESS` logic above will catch it. If you do not check the
        return value of ma_resource_manager_next_job() you will want to check for MA_RESOURCE_MANAGER_JOB_QUIT like the code below.
        */
        if (job.toc.breakup.code == MA_JOB_TYPE_QUIT) {
            printf("CUSTOM JOB THREAD TERMINATING VIA MA_RESOURCE_MANAGER_JOB_QUIT... ");
            break;
        }

        /* Call ma_resource_manager_process_job() to actually do the work to process the job. */
        printf("PROCESSING IN CUSTOM JOB THREAD: %d\n", job.toc.breakup.code);
        ma_resource_manager_process_job(pResourceManager, &job);
    }

    printf("TERMINATED\n");
    return (ma_thread_result)0;
}



#else

void Sound::soundTest1()
{
    printf("!!! Define EN_SOUND is disabled. Uncomment Define in sound.cpp if sound is desired !!! \n");
}

void Sound::soundTest2()
{
    printf("!!! Define EN_SOUND is disabled. Uncomment Define in sound.cpp if sound is desired !!! \n");
}

void Sound::soundTest3()
{
    printf("!!! Define EN_SOUND is disabled. Uncomment Define in sound.cpp if sound is desired !!! \n");
}

void Sound::soundInit()
{
    printf("!!! Define EN_SOUND is disabled. Uncomment Define in sound.cpp if sound is desired !!! \n");
}
#endif













