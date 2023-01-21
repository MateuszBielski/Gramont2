cmake_minimum_required(VERSION 3.10)
set (TargetName TestedClasses)
#cmake_path(SET path_src "${CMAKE_CURRENT_SOURCE_DIR}/../../src")#avaliable in cmake 3.20
set ( CXX_SRCS
    ../../src/Surface/surface
    ../../src/Surface/surfaceonequad.cpp
    ../../src/Surface/convexsurface.cpp
    ../../src/GeometryModel/onemodel.cpp
    ../../src/GeometryModel/triangle.cpp
    ../../src/Textures/textureformodel.cpp
    ../../src/OGLManagerVersatile/onemodelmanager.cpp
    ../../src/OGLManagerVersatile/multimodelmanager.cpp
    ../../src/Selecting/selecting.cpp
    ../../src/Selecting/selectingresult.cpp
    ../../src/ViewControl/cameratrial.cpp
    ../../src/ViewControl/matrixstack.cpp
    ../../src/Transform/transformable.cpp
    ../../lib/funcs/textfile.cpp
    ../../lib/funcs/funkcje.cpp
    ../../lib/oglstuff/oglstuff.cpp
    ../../lib/oglstuff/myoglmanager.cpp
    ../../lib/oglstuff/bufferloader.cpp
    ../../lib/oglstuff/oglrenderer.cpp
    ../../lib/mathstuff/mathstuff.cpp
    #../../../glad/src/glad.c
    )
set(Includes 
    #${path_src}
    ../../src
    ../../src/Surface 
    ../../src/GeometryModel
    ../../src/Textures
    ../../src/OGLManagerVersatile
    ../../src/Selecting
    ../../src/ViewControl
    ../../src/Transform
    ../../lib/oglstuff
    ../../lib/mathstuff
    ../../lib/funcs
)
add_library(LibGlad ../../../glad/src/glad.c)
target_include_directories(LibGlad 
            PUBLIC
            ../../../glad/include
            )
            
add_library(${TargetName} ${CXX_SRCS})

target_link_libraries(${TargetName} PUBLIC LibGlad HelperClasses)
target_include_directories(${TargetName} 
    PUBLIC 
    ${Includes}
    #PRIVATE
    #../../../glad/include
    )