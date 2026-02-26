cmake_minimum_required(VERSION 3.29)
project(Algorithms_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(Task07)

add_executable(Algorithms_Project
        Task07/globals.cpp
        Task07/Job.hpp
        Task07/job_processing.cpp
        Task07/job_processing_DP.cpp)
