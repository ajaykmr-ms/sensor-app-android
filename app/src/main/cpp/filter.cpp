#include "platform.h"
#include "filter.h"
#include <unistd.h>
#include <numeric>

static const char* kTAG = "Filter";

Filter::Filter()
{
    vx.reserve(WINDOW_STEP * 60);
    vy.reserve(WINDOW_STEP * 60);
    vz.reserve(WINDOW_STEP * 60);
    last = std::chrono::steady_clock::now();
    id = 0;
}

Filter::~Filter( )
{
}

void Filter::processSensorData(float x, float y, float z)
{
     vx.push_back(x);
     vy.push_back(y);
     vz.push_back(z);

    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(current - last).count();

    if ( us > WINDOW_STEP * 1000000)
    {
        //find the average for the current window
        float avg_x = average(vx);
        float avg_y = average(vy);
        float avg_z = average(vz);

        //Create a new window
        Window win;
        win.avg_x = avg_x;
        win.avg_y = avg_y;
        win.avg_z = avg_z;
        win.weightage = vx.size();
        win.id = id++;

        //if window length is full, remove first
        if (window.size() == WINDOW_LENGTH)
        {
            float curr_avg_x=0;
            float curr_avg_y=0;
            float curr_avg_z=0;
            long entries=0;
            for (const struct Window &w : window)
            {
                curr_avg_x += w.avg_x * w.weightage;
                curr_avg_y += w.avg_y * w.weightage;
                curr_avg_z += w.avg_z * w.weightage;
                entries += w.weightage;
            }
            curr_avg_x = curr_avg_x / entries;
            curr_avg_y = curr_avg_y / entries;
            curr_avg_z = curr_avg_z / entries;

            //print all windows, for debugging only
            //print_windows();
            //send the results to java
            sendSensorMsg(g_platform_context, curr_avg_x, curr_avg_y, curr_avg_z);

            //Now put the new window in our window vector
            window.erase(window.begin());
            window.push_back(win);
            //update the timestamp of last sensor update
            last = std::chrono::steady_clock::now();
            //clear current window
            vx.clear();
            vy.clear();
            vz.clear();
            vx.reserve(WINDOW_STEP * 60);
            vy.reserve(WINDOW_STEP * 60);
            vz.reserve(WINDOW_STEP * 60);
        }
        else
        {
            window.push_back(win);
        }
    }
}

float Filter::average(std::vector<float>& v)
{
    return (float) accumulate( v.begin(), v.end(), 0.0)/v.size();
}

void Filter::print_windows( )
{
    //some std::strings are not supported in the NDK version i am using hence c string functions
    char msg[512];
    msg[0]='\0';
    for (const struct Window &w : window)
    {
        char m[100];
        m[0]='\0';
        sprintf(m, "{id: %d(%d)}", w.id, w.weightage);
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
