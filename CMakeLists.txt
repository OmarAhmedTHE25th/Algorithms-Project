cmake_minimum_required(VERSION 3.29)
project(Algorithms_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(Task07)

add_executable(task7_DP
        Task07/globals.cpp
        Task07/Job.hpp
        Task07/job_processing_DP.cpp)
add_executable(
        task7_BF
        Task07/globals.cpp
        Task07/Job.hpp
        Task07/job_processing_BF.cpp
)
