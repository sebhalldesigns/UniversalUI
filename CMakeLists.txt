
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

 
  # Essential flags required in both compile and link phases
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_WEBGL2=1 -s USE_SDL=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -Wno-unused-command-line-argument")

    # Linker-only flags for fetching and WebGL version
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FETCH=1 -s MIN_WEBGL_VERSION=2")
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

    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_sdl2.cpp
    extern/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

target_include_directories(UniversalUI PRIVATE lib extern extern/imgui ${GSPCORE_PLATFORM_INCLUDE})

target_link_libraries(UniversalUI ${GSPCORE_PLATFORM_LIBRARIES})