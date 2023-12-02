# UniversalUI Native CMakeLists.txt

if(PLATFORM STREQUAL "ios")
    
    message("Configuring UniversalUI-Native for iOS...")

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
    message("Configuring UniversalUI-Native for macOS...")

    set(UNIVERSALUI_NATIVE_PLATFORM_SOURCE
        Source/macOS/nApplication.mm
    )

    set(PLATFORM_SPECIFIC_LIBRARIES
        "-framework Cocoa"
        "-framework Foundation"
    )
    

else()
    message(FATAL_ERROR "Unsupported platform ${PLATFORM}")
endif()



add_library(UniversalUI-Native STATIC ${UNIVERSALUI_NATIVE_PLATFORM_SOURCE})

target_include_directories(UniversalUI-Native PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Include/)

# Link platform-specific libraries
target_link_libraries(UniversalUI-Native PUBLIC ${PLATFORM_SPECIFIC_LIBRARIES})