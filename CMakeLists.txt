# linux dependencies libgl1-mesa-dev libx11-dev


#cmake_minimum_required(VERSION 3.25)
project(UniversalUI)

set(CMAKE_CXX_STANDARD 17)

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
else()
    message(FATAL_ERROR "Unsupported platform")
endif()




add_executable(UniversalUI
        Test/main.cpp
        Windowing/source/uWindow.cpp
        Core/source/UniversalUI.cpp
        Core/source/uApplication.cpp
        Windowing/source/uWindowManager.cpp
)

target_include_directories(UniversalUI PUBLIC "./Core/include")
target_include_directories(UniversalUI PUBLIC "./Bitmap/include")
target_include_directories(UniversalUI PUBLIC "./Windowing/include")
target_include_directories(UniversalUI PUBLIC "./Layout/include")


if(WIN32)
    message("Linking libraries for Windows")
    target_link_libraries(UniversalUI PRIVATE OpenGL::GL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Linking libraries for Linux")
    # Link against OpenGL and X11 libraries
    target_link_libraries(UniversalUI ${OPENGL_LIBRARIES} ${X11_LIBRARIES})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("Linking libraries for Darwin")
endif()
