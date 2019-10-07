cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(ANDROID_TOOLCHAIN "${ANDROID_TOOLCHAIN} gcc")

add_library(sensorapp-jni SHARED
            platform.c
            filter.cpp
            )

target_link_libraries(sensorapp-jni
                      android
                      log)
