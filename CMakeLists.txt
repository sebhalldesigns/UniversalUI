
if (WIN32)

    message(STATUS "Configuring for Windows")

    find_package(SDL2 REQUIRED)

    set(UNIVERSALUI_PLATFORM_SOURCE 

    )

    set(GSPCORE_PLATFORM_INCLUDE
        ${SDL2_INCLUDE_DIR}
    )

    set(GSPCORE_PLATFORM_LIBRARIES
        ${SDL2_LIBRARIES}
    )


elseif (APPLE)
    message(STATUS "Configuring for macOS")

elseif(EMSCRIPTEN)
    message(STATUS "Configuring for WASM")

    set(UNIVERSALUI_PLATFORM_SOURCE 

    )

   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -s \"FETCH=1\" -s \"USE_WEBGL2=1\" -s \"USE_SDL=2\" -s \"MIN_WEBGL_VERSION=2\"")
elseif (UNIX)
    message(STATUS "Configuring for Linux")


else()
    message(FATAL_ERROR "Unknown or unsupported platform!")
endif()

# Add the executable
add_library(UniversalUI STATIC
    lib/Application/UApplication.cpp
    lib/Window/UWindow.cpp
    lib/Log/ULog.cpp
)

target_include_directories(UniversalUI PRIVATE lib ${GSPCORE_PLATFORM_INCLUDE})

target_link_libraries(UniversalUI ${GSPCORE_PLATFORM_LIBRARIES})