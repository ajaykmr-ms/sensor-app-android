#include "platform.h"
#include "filter.h"
#include <unistd.h>

static const char* kTAG = "Filter";

Filter::Filter()
{
    w.acc_x = 0;
    w.acc_y = 0;
    w.acc_z = 0;
    w.entries = 0;
    last = std::chrono::steady_clock::now();
    id = 0;
}

Filter::~Filter( )
{
}

void Filter::processSensorData(float x, float y, float z)
{
    w.acc_x += x;
    w.acc_y += y;
    w.acc_z += z;
    w.entries++;

    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(current - last).count();

    if (us > WINDOW_STEP * 1000000)    // convert in micro-seconds
    {
        //Create a new window
        w.id = id++;

        //if window length is full, remove first
        if (windows.size() == WINDOW_LENGTH)
        {
            float curr_avg_x=0;
            float curr_avg_y=0;
            float curr_avg_z=0;
            long entries=0;

            for (const struct Window &w : windows)
            {
                curr_avg_x += w.acc_x;
                curr_avg_y += w.acc_y;
                curr_avg_z += w.acc_z;
                entries += w.entries;
            }
            curr_avg_x = curr_avg_x / entries;
            curr_avg_y = curr_avg_y / entries;
            curr_avg_z = curr_avg_z / entries;

            //print all windows, for debugging only
            //print_windows();

            //send the results to java
            sendSensorMsg(g_platform_context, curr_avg_x, curr_avg_y, curr_avg_z);

            //Now put the new window in our window vector
            windows.erase(windows.begin());
            windows.push_back(w);

            //update the timestamp of last sensor update
            last = std::chrono::steady_clock::now();
            //clear current window
            w.acc_x = 0;
            w.acc_y = 0;
            w.acc_z = 0;
            w.entries = 0;
        }
        else
        {
            windows.push_back(w);
        }
    }
}

void Filter::print_windows( )
{
    //some std::strings are not supported in the NDK version i am using hence c string functions
    char msg[512];
    msg[0]='\0';
    for (const struct Window &w : windows)
    {
        char m[100];
        m[0]='\0';
        sprintf(m, "{id: %d(%d)}", w.id, w.entries);
        strcat(msg, m);
    }
    LOGI("===> %s", msg);

}







//Define Native functions here to receive sensor data from Java
#ifdef __cplusplus
extern "C"
{
#endif

    JNIEXPORT jlong JNICALL Java_com_example_sensorapp_Filter_initializeFilter(JNIEnv *env, jclass obj)
    {
        Filter *filter = new Filter();
        jlong nativePtr = reinterpret_cast<jlong>(filter);
        return nativePtr;
    }

    JNIEXPORT void JNICALL Java_com_example_sensorapp_Filter_onSensorData(JNIEnv *env,
                                                                          jclass obj,
                                                                          jlong nativePtr,
                                                                          jfloat x,
                                                                          jfloat y,
                                                                          jfloat z )
    {
        Filter *filter = reinterpret_cast<Filter *>( nativePtr );
        filter->processSensorData(x,y,z);
        //sendSensorMsg(g_platform_context, x,y,z);
    }

    JNIEXPORT void JNICALL Java_com_example_sensorapp_Filter_releaseFilter(JNIEnv *env, jclass obj, jlong nativePtr)
    {
        Filter *filter = reinterpret_cast<Filter *>( nativePtr );
        //do anything else that needs to be done
        delete filter;
    }


#ifdef __cplusplus
}
#endif
