cmake_minimum_required(VERSION 3.16)

add_library(wxApp SHARED wxAppMain.cpp
myframe.cpp 
)

target_include_directories(wxApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
