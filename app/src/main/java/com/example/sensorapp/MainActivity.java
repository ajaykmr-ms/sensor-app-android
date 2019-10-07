package com.example.sensorapp;

import android.content.Context;
import android.os.Bundle;

import android.util.Log;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.view.View;
import androidx.annotation.Keep;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private static final String HTTP_SERVER = "https://enkpdwsit7bmo.x.pipedream.net/";
    private Context mContext;
    private SensorMgr mSensorMgr;
    private AppHttpClient mHttpClient=null;
    private boolean mStarted;

    int hour = 0;
    int minute = 0;
    int second = 0;
    TextView tickView;


    static {
        System.loadLibrary("sensorapp-jni");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mContext = this.getApplicationContext();
        setContentView(R.layout.activity_main);
        tickView = (TextView) findViewById(R.id.tickView);
        mStarted = false;
        final Button startButton = findViewById(R.id.button);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mStarted) {
                    mSensorMgr.stop();
                    startButton.setText("START");
                    mStarted = false;
                }
                else {
                    if (mSensorMgr == null) {
                        mSensorMgr = new SensorMgr(mContext);
                    }
                    mSensorMgr.start();
                    startButton.setText("STOP");
                    mStarted = true;
                }

            }
        });
    }
    @Override
    public void onResume() {
        super.onResume();
        hour = minute = second = 0;
        startTicks();
    }

    @Override
    public void onPause () {
        super.onPause();
        StopTicks();
    }

    /*
     * A function calling from JNI to update current timer
     */
    @Keep
    private void updateTimer() {
        ++second;
        if(second >= 60) {
            ++minute;
            second -= 60;
            if(minute >= 60) {
                ++hour;
                minute -= 60;
            }
        }
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String ticks = "" + MainActivity.this.hour + ":" +
                        MainActivity.this.minute + ":" +
                        MainActivity.this.second;
                MainActivity.this.tickView.setText(ticks);
            }
        });
    }

    @Keep
    public void sendFilteredData(float x, float y, float z) {
        //Commented out for testing
        if (mHttpClient == null)
        {
            mHttpClient = new AppHttpClient(HTTP_SERVER);
        }
        String dataStr = "{\"ax\":" + x + ", \"ay\":" + y + ", \"az\":" + z + "}";
        //Log.v("===>", dataStr);
        mHttpClient.sendHttpRequest(dataStr);
    }

    /* Declare native functions here */
    public native  String stringFromJNI();
    public native void startTicks();
    public native void StopTicks();
}
