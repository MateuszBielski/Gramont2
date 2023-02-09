cmake_minimum_required(VERSION 3.10)

project(Gramont2Test)
find_package(wxWidgets REQUIRED gl core)
#find_package(GTest REQUIRED PATHS /home/mateusz/gTest NO_DEFAULT_PATH)
include(FindPkgConfig) #ok
#pkg_search_module(GTEST REQUIRED gtest)#ok
#find_package(GTest REQUIRED)
pkg_check_modules (GTEST REQUIRED gtest)


include(${wxWidgets_USE_FILE})

set(CMAKE_BUILD_TYPE Debug)



set ( CXX_SRCS
    main.cpp
    src/TransformableTest.cpp
    src/SurfaceTest.cpp
    src/TextureForModelTest.cpp
    src/OGLmanagerTest.cpp
    src/SelectingTest.cpp
    src/ShaderTest.cpp
    src/OglRendererTest.cpp
    src/BufferLoaderTest.cpp
    src/ViewControlTest.cpp
    src/ViewControlNeedFixTest.cpp
)
if(UNIX)
#add_compile_options(-H)
    add_compile_options(-fPIC)#static Glad is linked to dynamic Oglstuff
endif(UNIX)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/oglstuff ${CMAKE_CURRENT_BINARY_DIR}/oglstuff)
add_subdirectory(libTestedClasses)
add_subdirectory(libHelperClasses)

add_executable(${CMAKE_PROJECT_NAME} ${CXX_SRCS} )

target_link_libraries(${CMAKE_PROJECT_NAME} 
        PUBLIC
        HelperClasses
        TestedClasses
        -L${GTEST_LIBRARY_DIRS}
        ${GTEST_LIBRARIES}
        ${wxWidgets_LIBRARIES}
        )
target_include_directories(${CMAKE_PROJECT_NAME} 
        PRIVATE
        #/home/mateusz/gTest/include
        ${GTEST_INCLUDE_DIRS}
        )
target_compile_definitions(
        TestedClasses
        PUBLIC 
        TESTOWANIE_F
        )
target_compile_definitions(
        OglStuff
        PUBLIC 
        TESTOWANIE_F
        )
