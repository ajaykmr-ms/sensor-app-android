package com.example.sensorapp;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.LocationListener;
import android.util.Log;

//import static java.lang.Math.cos;
//import static java.lang.Math.sin;
//import static java.lang.Math.sqrt;

public class SensorMgr implements SensorEventListener, LocationListener {
    private static final String TAG = "SensorMgr";
    private Context mContext;
    private SensorManager mSensorMgr = null;
    private Sensor mAccelerometer = null;
    private int mSensorDelay = SensorManager.SENSOR_DELAY_GAME;
    private Filter mFilter = null;

    SensorMgr(Context context) {
        mContext = context;
        mSensorMgr = (SensorManager) mContext.getSystemService(Context.SENSOR_SERVICE);
        mFilter = new Filter();
    }

    public void start() {
        mAccelerometer = mSensorMgr.getDefaultSensor( Sensor.TYPE_ACCELEROMETER );
        if(mAccelerometer != null)
        {
            mSensorMgr.registerListener( this, mAccelerometer, mSensorDelay );
        }
    }

    public void stop() {
        if(mAccelerometer != null)
        {
            mSensorMgr.unregisterListener( this, mAccelerometer );
        }
    }

    @Override
    public void onAccuracyChanged(Sensor arg0, int arg1)
    {
        // TODO Auto-generated method stub
    }

    @Override
    public void onSensorChanged(SensorEvent event)
    {

        long ts = event.timestamp;

        switch( event.sensor.getType() )
        {
            case Sensor.TYPE_ACCELEROMETER:
                float[] acc = event.values;
                float accx = event.values[0];
                float accy = event.values[1];
                float accz = event.values[2];

                mFilter.onAccelerometerData(accx, accy, accz);

                break;
            default:
        }
    }

    // GPS data
    @Override
    public void onLocationChanged(android.location.Location location)
    {
        Log.v(TAG, "onLocationChanged");
    }

    @Override
    public void onProviderDisabled(String provider)
    {
        Log.v(TAG, "onProviderDisabled");
    }

    @Override
    public void onProviderEnabled(String provider)
    {
        Log.v(TAG, "onProviderEnabled");
    }

    @Override
    public void onStatusChanged(String provider, int status, android.os.Bundle extras)
    {
        Log.v(TAG, "onStatusChanged");
    }
}
