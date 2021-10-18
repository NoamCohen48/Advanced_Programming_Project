cmake_minimum_required(VERSION 3.20)
project(Project1)

set(CMAKE_CXX_STANDARD 11)

add_executable(Time_series_data anomaly_detection_util.cpp anomaly_detection_util.h MainTest.cpp MainTest.h)
