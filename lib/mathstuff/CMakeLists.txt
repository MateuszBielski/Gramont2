cmake_minimum_required(VERSION 3.16)

add_library(MathStuff SHARED mathstuff)

target_include_directories(MathStuff PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
