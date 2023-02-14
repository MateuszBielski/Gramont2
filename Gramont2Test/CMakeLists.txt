cmake_minimum_required(VERSION 3.10)

project(Gramont2Test)

include(FetchContent)

if (MSVC)
set(wxWidgets_ROOT_DIR D:\\programy\\wxWidgets)
set(wxWidgets_CONFIGURATION mswud)
# set(GTEST_ROOT_DIR D:\\programy\\googletest1.13\\googletest)
# set(GTEST_LIBRARY D:\\programy\\googletest1.13\\build\\lib\\Debug\\gtest.lib)
# set(GTEST_MAIN_LIBRARY D:\\programy\\googletest1.13\\build\\lib\\Debug\\gtest_main.lib)
# set(GTEST_INCLUDE_DIR D:\\programy\\googletest1.13\\googletest\\include)
# https://matgomes.com/integrate-google-test-into-cmake/
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
endif (MSVC)

find_package(wxWidgets REQUIRED gl core base)
#find_package(GTest REQUIRED PATHS /home/mateusz/gTest NO_DEFAULT_PATH)
if(UNIX)
include(FindPkgConfig) #ok
pkg_check_modules (GTEST REQUIRED gtest)
endif(UNIX)

# find_package(GTest REQUIRED)


include(${wxWidgets_USE_FILE})
if (MSVC)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULT:LIBCMTD")
	# set(
	set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
	#add_compile_options(/W0)
    # add_compile_options(/showIncludes)
endif (MSVC)

set(CMAKE_CXX_STANDARD 20)
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
add_compile_definitions(_USE_MATH_DEFINES)#for using M_PI constant
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/oglstuff ${CMAKE_CURRENT_BINARY_DIR}/oglstuff)
add_subdirectory(libTestedClasses)
add_subdirectory(libHelperClasses)

add_executable(${CMAKE_PROJECT_NAME} ${CXX_SRCS} )

if(UNIX)
target_link_libraries(${CMAKE_PROJECT_NAME} 
        PUBLIC
        HelperClasses
        TestedClasses
        -L${GTEST_LIBRARY_DIRS}
        ${GTEST_LIBRARIES}
        ${wxWidgets_LIBRARIES}
        )
endif(UNIX)

if(MSVC)
target_link_libraries(${CMAKE_PROJECT_NAME} 
        PUBLIC
		HelperClasses
        TestedClasses
		GTest::GTest 
		${wxWidgets_LIBRARIES}
		)

# add_test(multiply_gtests multiply_test)
endif(MSVC)
# target_include_directories(${CMAKE_PROJECT_NAME} 
        # PRIVATE
        #/home/mateusz/gTest/include
        # ${GTEST_INCLUDE_DIRS}
        # )
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