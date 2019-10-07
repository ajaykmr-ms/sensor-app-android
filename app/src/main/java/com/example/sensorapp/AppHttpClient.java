package com.example.sensorapp;

import android.util.Log;

import javax.net.ssl.HttpsURLConnection;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class AppHttpClient {
    private static final String TAG = "AppHttpClient";
    private URL mUrl;

    AppHttpClient(String stringUrl)
    {
        Log.v(TAG, "AppHttpClient");
        try {
            mUrl = new URL(stringUrl);
        } catch (MalformedURLException ex) {
            Log.e(TAG, ex.getMessage());
        }
    }

    public void sendHttpRequest(final String buffer)
    {
        Thread httpRequestThread = new Thread()
        {
            @Override
            public void run() {
                HttpURLConnection conn = null;
                BufferedReader bufReader = null;
                InputStreamReader strReader = null;
                StringBuffer respText = new StringBuffer();
                try {
                    conn = (HttpURLConnection)mUrl.openConnection();

                    if (false) {
                        conn.setRequestMethod("GET");
                        conn.setConnectTimeout(10000);
                        conn.setReadTimeout(10000);
                    } else {
                        // setup request to send JSON data
                        conn.setRequestMethod("POST");
                        conn.setRequestProperty("Content-Type", "application/json;charset=utf-8");
                        conn.setRequestProperty("Accept", "application/json");
                        conn.setDoOutput(true);
                        conn.setDoInput(true);

                        DataOutputStream os = new DataOutputStream(conn.getOutputStream());
                        //    byte[] input = buffer.getBytes("utf-8");
                        os.writeBytes(buffer);
                        os.flush();
                        os.close();

                        Log.i("STATUS", String.valueOf(conn.getResponseCode()));
                        Log.i("MSG" , conn.getResponseMessage());
                    }

                    InputStream inputStream = conn.getInputStream();
                    strReader = new InputStreamReader(inputStream);
                    bufReader = new BufferedReader(strReader);

                    //Read response from server
                    String line = null;

                    while ((line = bufReader.readLine()) != null)
                    {
                        respText.append(line);
                    }

                    //Print the response
                    Log.v("MY RESPNSE", respText.toString());
                } catch (IOException ex) {
                    Log.e(TAG, ex.getMessage());
                } finally {
                    try {
                        if (bufReader != null) {
                            bufReader.close();
                            bufReader = null;
                        }
                        if (strReader != null) {
                            strReader.close();
                            strReader = null;
                        }
                        if (conn != null) {
                            conn.disconnect();
                            conn = null;
                        }
                    } catch (IOException ex) {
                        Log.e(TAG, ex.getMessage());
                    }
                }

            }
        };
        httpRequestThread.start();
    }




}
