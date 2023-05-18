cmake_minimum_required(VERSION 3.10)
set (TargetName HelperClasses)
#cmake_path(SET path_src "${CMAKE_CURRENT_SOURCE_DIR}/../../src")#avaliable in cmake 3.20
set ( CXX_SRCS
    glFunctionsMock.cpp
    selectingtestaccess.cpp
    glshadersmock.cpp
    onemodelmock.cpp
    MultiModelManagerAccess.cpp
    OglRendererMock.cpp
    matrixstackmock.cpp
    bufferloadermock.cpp
    rendersystemmock.cpp
    )
set(Includes 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(${TargetName} ${CXX_SRCS})

target_link_libraries(${TargetName} PUBLIC 
TestedClasses
OglStuff
)

target_include_directories(${TargetName} 
    PUBLIC 
    ${Includes}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../include
    )