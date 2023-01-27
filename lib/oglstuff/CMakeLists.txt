cmake_minimum_required(VERSION 3.16)

add_library(OglStuff 
SHARED 
oglstuff.cpp
myoglmanager.cpp
oglrenderer.cpp
bufferloader.cpp
#dwa powyższe może da się złączyć?
)

target_link_libraries(OglStuff PRIVATE MathStuff Glad )
target_include_directories(OglStuff PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#target_compile_definitions(
#       OglStuff
#       PUBLIC 
#       GLIBCXX_DEBUG
        # powoduje błąd linkowania dla funkcji template void myOGLManager::setLocations<T>
#      )
