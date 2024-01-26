# UniversalUI Graphics CMakeLists.txt

if (CMAKE_SYSTEM_NAME STREQUAL "DARWIN")

elseif (WIN32) 

    set(UNIVERSALUI_NATIVE_GRAPHICS_SOURCE
        Source/Win32/uRenderSurface.cpp
        Source/Win32/uFont.cpp
    )

    find_package(OpenGL REQUIRED)

    set(PLATFORM_SPECIFIC_GRAPHICS_LIBRARIES
        OpenGL::GL
        "${CMAKE_CURRENT_SOURCE_DIR}/Lib/windows-x64/freetype.lib"
    )

else()
    message(FATAL_ERROR "Unsupported platform ${CMAKE_SYSTEM_NAME}")
endif()

add_library(UniversalUI-Graphics STATIC ${UNIVERSALUI_NATIVE_GRAPHICS_SOURCE})

target_include_directories(UniversalUI-Graphics PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/freetype2)

target_link_libraries(UniversalUI-Graphics PUBLIC ${PLATFORM_SPECIFIC_GRAPHICS_LIBRARIES})