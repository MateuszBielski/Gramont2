cmake_minimum_required(VERSION 3.16)

add_library(Funcs SHARED funkcje.cpp textfile.cpp)

target_link_libraries(Funcs PRIVATE Glad OglStuff MathStuff)
target_include_directories(Funcs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
