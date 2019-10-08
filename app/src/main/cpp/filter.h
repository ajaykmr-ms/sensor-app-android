
#include "platform.h"
#include <iostream>
#include <vector>
#include <thread>

#define WINDOW_STEP 1
#define WINDOW_LENGTH 5

class Filter
{
    struct Window
    {
        float acc_x;    //accumulate x
        float acc_y;    //accumulate y
        float acc_z;    //accumulate z
        long entries;   //number of sensor inputs in the window
        int id;         //window id - for debugging only
    };

    public:
        Filter();
        ~Filter();

       void processSensorData(float x, float y, float z);


    private:
        struct Window w;                                        //current window
        std::vector<struct Window> windows;                     //store all the previous windows
        std::chrono::steady_clock::time_point last;             //track the last update
        int id;                                                 //window id
        void print_windows();
};
