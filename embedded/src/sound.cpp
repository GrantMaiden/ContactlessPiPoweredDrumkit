/****************************** Module Header ******************************\
Module Name:    sound.cpp
Project:        btb
Author:         Grant Maiden
Description:    sound control functions and processes
\***************************************************************************/
#define MA_NO_ENGINE
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


/**********************************************\
Function Name:  soundInit
Input Args:     none
Output Args:    void
Description:    initialize sound variables
/**********************************************/
void Sound::soundInit()
{

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
Function Name:  soundTest3
Input Args:     none
Output Args:    void
Description:    test Sound engine
/**********************************************/
void Sound::soundTest3()
{

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
#endif














