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
    src/OneModelTest.cpp
    src/TextureForModelTest.cpp
    src/OGLmanagerTest.cpp
    src/SelectingTest.cpp
    src/ShaderTest.cpp
    src/OglRendererTest.cpp
    src/BufferLoaderTest.cpp
    src/ViewControlTest.cpp
    src/ViewControlNeedFixTest.cpp
)
#add_compile_options(-H)
#target_compile_options(xxx?)
add_subdirectory(libTestedClasses)
add_subdirectory(libHelperClasses)

add_executable(${CMAKE_PROJECT_NAME} ${CXX_SRCS} )
#target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE )
target_link_libraries(${CMAKE_PROJECT_NAME} 
        PUBLIC
        HelperClasses
        TestedClasses
        #GTest::GTest
        #-L/home/mateusz/gTest/lib -lgtest -lgtest_main -pthread
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
        #${CMAKE_PROJECT_NAME}
        TestedClasses
        PUBLIC 
        TESTOWANIE_F
        )
