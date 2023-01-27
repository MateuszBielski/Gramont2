cmake_minimum_required(VERSION 3.16)

add_library(Glad SHARED ../../../glad/src/glad.c)

target_include_directories(Glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../glad/include)
