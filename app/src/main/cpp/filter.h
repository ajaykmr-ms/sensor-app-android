
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
        float avg_x;
        float avg_y;
        float avg_z;
        long weightage;
        int id;    //for debugging only
    };

    public:
        Filter();
        ~Filter();

       void processSensorData(float x, float y, float z);


    private:
        //three scratch vectors for maintaining current window
        std::vector<float> vx;
        std::vector<float> vy;
        std::vector<float> vz;
        std::vector<struct Window> window;                      //store all the previous windows
        std::chrono::steady_clock::time_point last;             //track the last update
        int id;                                                 //window id
        float average(std::vector<float>& v);
        void print_windows();
};
