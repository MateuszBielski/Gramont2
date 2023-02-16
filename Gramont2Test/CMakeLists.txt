#https://www.testcookbook.com/book/cpp/setting-up-cmake-google-test.html
cmake_minimum_required(VERSION 3.16)
project(Gramont2Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)# włączyć gdy zadziała pozostałe

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
	set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
	
	#add_compile_options(/W0)
    # add_compile_options(/showIncludes)
endif (MSVC)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#dodatkowe informacje znalezione później
#https://stackoverflow.com/questions/12540970/how-to-make-gtest-build-mdd-instead-of-mtd-by-default-using-cmake

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
            "${gmock_SOURCE_DIR}/include")
endif()

if (MSVC)
set(wxWidgets_ROOT_DIR D:\\programy\\wxWidgets)
set(wxWidgets_CONFIGURATION mswud)
endif (MSVC)

find_package(wxWidgets REQUIRED gl core base)
include(${wxWidgets_USE_FILE})

enable_testing()

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

#include_directories("${PROJECT_SOURCE_DIR}")


add_executable(${CMAKE_PROJECT_NAME} ${CXX_SRCS} )
target_link_libraries(${CMAKE_PROJECT_NAME} 
        PUBLIC
        HelperClasses
        TestedClasses
        gtest
        gmock_main
        ${wxWidgets_LIBRARIES}
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
#gtest_add_tests(TARGET ${CMAKE_PROJECT_NAME})    

#add_test(NAME simple_test COMMAND ${CMAKE_PROJECT_NAME})

