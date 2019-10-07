package com.example.sensorapp;

import android.util.Log;

public class Filter {
    private static final String TAG = "Filter";

    private native static long initializeFilter();
    private native static void onSensorData(long nativePtr, float x, float y, float z);
    private native static void releaseFilter(long nativePtr);
    private long mNativePtr=0;

    Filter() {
        Log.v(TAG, "Initialized");
        mNativePtr = initializeFilter();
    }

    void onAccelerometerData(float x, float y, float z) {
        //indicate the sensor data to c++ core
        onSensorData(mNativePtr, x,y,z);
    }

    void stop() {
        releaseFilter(mNativePtr);
        mNativePtr = 0;
        Log.v(TAG, "Released");
    }
};
