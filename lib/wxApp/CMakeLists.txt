cmake_minimum_required(VERSION 3.16)

if(MSVC)
add_library(wxApp STATIC wxAppMain.cpp
myframe.cpp 
)
else()
add_library(wxApp SHARED wxAppMain.cpp
myframe.cpp 
)
endif(MSVC)

target_include_directories(wxApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
