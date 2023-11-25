cmake_minimum_required(VERSION 3.26)
project(UniversalUI)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)


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


target_link_libraries(UniversalUI PRIVATE OpenGL::GL)