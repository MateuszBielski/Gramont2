cmake_minimum_required(VERSION 3.16)

add_library(OglStuff 
SHARED 
oglstuff.cpp
myoglmanager.cpp
)
add_subdirectory(glad)
add_subdirectory(mathstuff)
target_link_libraries(OglStuff PUBLIC MathStuff Glad)
target_include_directories(OglStuff PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#target_compile_definitions(
#       OglStuff
#       PUBLIC 
#       GLIBCXX_DEBUG
        # powoduje błąd linkowania dla funkcji template void myOGLManager::setLocations<T>
#      )
