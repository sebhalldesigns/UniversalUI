# linux dependencies libgl1-mesa-dev libx11-dev


cmake_minimum_required(VERSION 3.5)
project(UniversalUI)
add_definitions(-w)

# Check if the target platform is Windows
if(WIN32)
    message("Configuring for Windows")
    find_package(OpenGL REQUIRED)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Configuring for Linux")
    # Find OpenGL
    find_package(OpenGL REQUIRED)
    # Find X11
    find_package(X11 REQUIRED)
    # Include directories for OpenGL and X11
    include_directories(${OPENGL_INCLUDE_DIRS} ${X11_INCLUDE_DIR})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("Configuring for Darwin")
    project(UniversalUI LANGUAGES CXX OBJCXX)

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)

else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES 
    Test/main.cpp
    Windowing/source/uWindow.cpp
    Core/source/UniversalUI.cpp
    Core/source/uApplication.cpp
    Windowing/source/uWindowManager.cpp
    Rendering/source/uCanvas.cpp
    Rendering/source/uRenderNode.cpp
)

add_executable(UniversalUI ${SOURCE_FILES})

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE OBJCXX)
endif()

target_include_directories(UniversalUI PUBLIC "./Core/include")
target_include_directories(UniversalUI PUBLIC "./Windowing/include")
target_include_directories(UniversalUI PUBLIC "./Layout/include")
target_include_directories(UniversalUI PUBLIC "./Rendering/include")

target_include_directories(UniversalUI SYSTEM PRIVATE "./Rendering/include/freetype2")


if(WIN32)
    message("Linking libraries for Windows")
    target_link_libraries(UniversalUI PRIVATE OpenGL::GL)
    
    target_link_libraries(UniversalUI PRIVATE "${CMAKE_SOURCE_DIR}/Rendering/lib/freetype.lib")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Linking libraries for Linux")
    # Link against OpenGL and X11 libraries

    target_link_libraries(UniversalUI PRIVATE ${OPENGL_LIBRARIES} ${X11_LIBRARIES})

    include_directories(${CMAKE_SOURCE_DIR}/Rendering/lib)
    target_link_libraries(UniversalUI PRIVATE ${CMAKE_SOURCE_DIR}/Rendering/lib/libfreetype.a)
    

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("Linking libraries for Darwin")
    target_link_libraries(UniversalUI ${COCOA_LIBRARY} ${OPENGL_LIBRARY})
endif()
