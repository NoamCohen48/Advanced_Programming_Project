cmake_minimum_required(VERSION 3.20)
project(Project2)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project2
        anomaly_detection_util.cpp anomaly_detection_util.h
        SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h
        timeseries.cpp timeseries.h
        MainTrain.cpp
        minCircle.cpp minCircle.h
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h
        CLI.cpp CLI.h
        commands.h)
