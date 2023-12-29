cmake_minimum_required(VERSION 3.16)


set(includes 
    ${CMAKE_CURRENT_SOURCE_DIR}
    Surface
    OGLManagerVersatile
    Textures
    GeometryModel
    ViewControl
    Transform
)

set ( CXX_SRCS
    myglcanvas.cpp
	myglcanvasDependencyInjection.cpp
    Surface/surface.cpp
    Surface/convexsurface.cpp
    OGLManagerVersatile/onemodelmanager.cpp
    OGLManagerVersatile/multimodelmanager.cpp
    GeometryModel/onemodel.cpp
    Textures/textureformodel.cpp
    ViewControl/cameratrial.cpp
    ViewControl/matrixstack.cpp
    Transform/transformable.cpp
)
set(CMAKE_ENABLE_EXPORTS ON)
add_executable(${CMAKE_PROJECT_NAME} ${CXX_SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} 
    PRIVATE 
    ${wxWidgets_LIBRARIES} 
    Glad
    MathStuff
    OglStuff
    Funcs
    wxApp
    )
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${includes})
target_include_directories(OglStuff PRIVATE ${includes})    
target_include_directories(wxApp PRIVATE ${includes})
