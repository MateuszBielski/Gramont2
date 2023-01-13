cmake_minimum_required(VERSION 3.10)

project(Gramont2)

if (MSVC)
set(wxWidgets_ROOT_DIR D:\\programy\\wxWidgets)
set(wxWidgets_CONFIGURATION mswud)
endif (MSVC)

find_package(wxWidgets REQUIRED gl core)
include(${wxWidgets_USE_FILE})

set(includes 
    src
    src/Surface
    src/OGLManagerVersatile
    src/Textures
    src/GeometryModel
    src/ViewControl
    src/Transform
)
#message("standard c++")
#message(STATUS ${CMAKE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD 20)

set ( CXX_SRCS
    src/myglcanvas.cpp
    src/Surface/surface.cpp
    src/Surface/convexsurface.cpp
    src/OGLManagerVersatile/onemodelmanager.cpp
    src/OGLManagerVersatile/multimodelmanager.cpp
    src/GeometryModel/onemodel.cpp
    src/Textures/textureformodel.cpp
    src/ViewControl/cameratrial.cpp
    src/ViewControl/matrixstack.cpp
    src/Transform/transformable.cpp
)
set(CMAKE_BUILD_TYPE Debug)
#add_compile_options(-H)

add_subdirectory(lib/glad)
add_subdirectory(lib/mathstuff)
add_subdirectory(lib/oglstuff)
add_subdirectory(lib/funcs)
add_subdirectory(lib/wxApp)

if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif (MSVC)

add_executable(${CMAKE_PROJECT_NAME} ${CXX_SRCS})
target_link_libraries(${CMAKE_PROJECT_NAME} 
    PUBLIC 
    ${wxWidgets_LIBRARIES} 
    Glad
    MathStuff
    OglStuff
    Funcs
    wxApp
    )
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${includes})
target_include_directories(OglStuff 
            PRIVATE 
            src/GeometryModel
            src/Textures
            )
#target_compile_definitions(
#       ${CMAKE_PROJECT_NAME}
#       PUBLIC 
#      GLIBCXX_DEBUG#nie widać działania
#      )
#set(CMAKE_BUILD_TYPE Release) ustawia dla wszytkich wykonywalnych

            

    