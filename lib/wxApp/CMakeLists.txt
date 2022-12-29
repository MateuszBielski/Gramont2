cmake_minimum_required(VERSION 3.10)

add_library(wxApp wxAppMain.cpp
myframe.cpp 
#myglcanvas.cpp
)
#target_link_libraries(wxApp PRIVATE Funcs Glad OglStuff MathStuff)

target_include_directories(wxApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}#Private?
	#../../src/OGLManagerVersatile
	#../../src/GeometryModel
	)