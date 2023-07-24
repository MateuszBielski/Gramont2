cmake_minimum_required(VERSION 3.16)

project(Gramont2)

if (MSVC)
set(wxWidgets_ROOT_DIR D:\\programy\\wxWidgets)
set(wxWidgets_CONFIGURATION mswud)
endif (MSVC)

find_package(wxWidgets REQUIRED gl core base)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

#

if (MSVC)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
	#add_compile_options(/W0)
    # add_compile_options(/showIncludes)
endif (MSVC)
if(UNIX)
    #add_compile_options(-H)
    add_compile_options(-fPIC)#static Glad is linked to dynamic Oglstuff
endif(UNIX)

set(includes 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
	../include
	lib/funcs
    src/OGLManagerVersatile
    src/GeometryModel
    src/Textures
    src/Transform
    src/ViewControl
    src/Surface
    src/Selecting
    src/Rendering
)

set ( CXX_SRCS
    src/myglcanvas.cpp
	lib/funcs/funkcje.cpp
	lib/funcs/textfile.cpp
    src/OGLManagerVersatile/modelmanager.cpp
    src/OGLManagerVersatile/multimodelmanager.cpp
    src/OGLManagerVersatile/oglrenderer.cpp
    src/OGLManagerVersatile/bufferloader.cpp
    src/Textures/textureinmemory.cpp
    src/Textures/textureformodel.cpp
    src/Transform/transformable.cpp
    src/Surface/surface.cpp
    src/Surface/convexsurface.cpp
    src/GeometryModel/onemodel.cpp
    src/ViewControl/matrixstack.cpp
    src/ViewControl/cameratrial.cpp
    src/Selecting/selecting.cpp
    src/Selecting/selectingresult.cpp
    src/Selecting/pickingrenderer.cpp
    src/Selecting/pickingbuffloader.cpp
    src/Selecting/selectable.cpp
    src/Rendering/rendersystem.cpp
    src/Rendering/onetexturerendersystem.cpp
    src/Rendering/paralaxoclusionmaprendersystem.cpp
    src/Rendering/pomrenderer.cpp
)
add_compile_definitions(_USE_MATH_DEFINES)#for using M_PI constant
add_executable(${CMAKE_PROJECT_NAME} ${CXX_SRCS})
#add_subdirectory(lib/glad)
#add_subdirectory(lib/mathstuff)
add_subdirectory(lib/oglstuff)
# add_subdirectory(lib/funcs)
add_subdirectory(lib/wxApp)
# add_subdirectory(src)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    PUBLIC 
    ${wxWidgets_LIBRARIES}
    #Glad
    OglStuff
    wxApp
    #MathStuff
	)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${includes})
target_include_directories(wxApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
#target_include_directories(OglStuff 
#            PRIVATE 
 #           src/GeometryModel
 #           src/Textures
 #           )
#target_compile_definitions(
 #      ${CMAKE_PROJECT_NAME}
 #      PUBLIC 
 #     GLIBCXX_DEBUG#nie widać działania
 #     )
#set(CMAKE_BUILD_TYPE Release) ustawia dla wszytkich wykonywalnych

            

    
