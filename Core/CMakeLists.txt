# UniversalUI Core CMakeLists.txt

if(PLATFORM STREQUAL "ios")
    
    message("Configuring UniversalUI-Core for iOS...")

    # Specify the minimum iOS version and architecture
    #set(CMAKE_SYSTEM_NAME iOS)  
    #set(CMAKE_OSX_SYSROOT "iphoneos")
    #set(CMAKE_OSX_ARCHITECTURES "arm64")

    set(UNIVERSALUI_NATIVE_PLATFORM_SOURCE
        Source/iOS/nApplication.mm
    )

    set(PLATFORM_SPECIFIC_LIBRARIES
        "-framework UIKit"
        "-framework Foundation"
    )
    

elseif(PLATFORM STREQUAL "macos")
    # Set the target platform to macOS
    set(CMAKE_SYSTEM_NAME MacOSX)
    message("Configuring UniversalUI-Core for macOS...")

    set(UNIVERSALUI_NATIVE_PLATFORM_SOURCE
        Source/macOS/nApplication.mm
    )

    set(PLATFORM_SPECIFIC_LIBRARIES
        "-framework Cocoa"
        "-framework Foundation"
    )

elseif(WIN32)
    message("Configuring UniversalUI-Core for Windows...")

    set(UNIVERSALUI_NATIVE_PLATFORM_SOURCE
        Source/Win32/uApplication.cpp
    )


    find_package(OpenGL REQUIRED)

    set(PLATFORM_SPECIFIC_LIBRARIES
        OpenGL::GL
    )
else()
    message(FATAL_ERROR "Unsupported platform ${PLATFORM}")
endif()



add_library(UniversalUI-Core STATIC ${UNIVERSALUI_NATIVE_PLATFORM_SOURCE})

target_include_directories(UniversalUI-Core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/)

# Link platform-specific libraries
target_link_libraries(UniversalUI-Core PUBLIC ${PLATFORM_SPECIFIC_LIBRARIES})