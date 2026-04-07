cmake_minimum_required(VERSION 3.31)
project(PowerQualityAnalyser C)

set(CMAKE_C_STANDARD 99)

add_executable(PowerQualityAnalyser main.c io.c waveform.c)

target_link_libraries(PowerQualityAnalyser m)