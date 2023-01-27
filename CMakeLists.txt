cmake_minimum_required(VERSION 3.16)

project(Gramont2)

if (MSVC)
set(wxWidgets_ROOT_DIR D:\\programy\\wxWidgets)
set(wxWidgets_CONFIGURATION mswud)
endif (MSVC)

find_package(wxWidgets REQUIRED gl core)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

#add_compile_options(-H)

if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif (MSVC)

add_subdirectory(lib/glad)
add_subdirectory(lib/mathstuff)
add_subdirectory(lib/oglstuff)
add_subdirectory(lib/funcs)
add_subdirectory(lib/wxApp)
add_subdirectory(src)



#target_include_directories(OglStuff 
#            PRIVATE 
 #           src/GeometryModel
 #           src/Textures
 #           )
#target_compile_definitions(
#       ${CMAKE_PROJECT_NAME}
#       PUBLIC 
#      GLIBCXX_DEBUG#nie widać działania
#      )
#set(CMAKE_BUILD_TYPE Release) ustawia dla wszytkich wykonywalnych

            

    
