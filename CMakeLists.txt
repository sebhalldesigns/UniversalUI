# linux dependencies libgl1-mesa-dev libx11-dev

cmake_minimum_required(VERSION 3.5)

project(UniversalUI)

add_definitions(-w)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    message("Building UniversalUI for Windows...")

    #find_package(GSP-System)

    add_executable(UniversalUI main.cpp)


    find_package(OpenGL REQUIRED)
    find_package(Vulkan REQUIRED)


    set(GSP_GRAPHICS_LIBRARIES 
        OpenGL::GL
        Vulkan::Vulkan
    )

    target_link_libraries(
        UniversalUI ${CMAKE_SOURCE_DIR}/../GSP/src/build/GSP-Graphics/Debug/GSP-Graphics.lib 
        ${CMAKE_SOURCE_DIR}/../GSP/src/build/GSP-System/Debug/GSP-System.lib
        ${GSP_GRAPHICS_LIBRARIES}
    
    )
    target_include_directories(UniversalUI PRIVATE  ${CMAKE_SOURCE_DIR}/../GSP/src/GSP-System/include  ${CMAKE_SOURCE_DIR}/../GSP/src/GSP-Graphics/include)

endif()
