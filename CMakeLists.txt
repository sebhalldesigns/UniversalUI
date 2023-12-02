# linux dependencies libgl1-mesa-dev libx11-dev

cmake_minimum_required(VERSION 3.5)
project(UniversalUI)

set(PLATFORM "ios")

if(PLATFORM STREQUAL "ios")
    
    message("Configuring UniversalUI for iOS...")

    # Specify the minimum iOS version and architecture
    set(CMAKE_SYSTEM_NAME iOS)  
    set(CMAKE_OSX_SYSROOT "iphoneos")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")


    add_subdirectory(Core)
    add_subdirectory(Native)

    add_executable(UniversalUI MACOSX_BUNDLE main.cpp)
    target_link_libraries(UniversalUI PRIVATE UniversalUI-Core)

    set(IOS_RESOURCE_FILES 
        External/Config/iOS/Info.plist
        External/Config/iOS/Assets.xcassets
        External/Config/iOS/Base.lproj
    )

    # Set the app bundle identifier and other iOS-specific settings
    set_target_properties(UniversalUI PROPERTIES
        BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.myapp"
        MACOSX_BUNDLE_BUNDLE_NAME "MyiOSApp"
        MACOSX_BUNDLE_BUNDLE_VERSION "1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.0.1"
        MACOSX_BUNDLE_LONG_VERSION_STRING "0.0.1"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.yourcompany.myapp"
        XCODE_ATTRIBUTE_PRODUCT_NAME "MyiOSApp"
        XCODE_ATTRIBUTE_PRODUCT_TYPE "com.apple.product-type.application"
        RESOURCE "${IOS_RESOURCE_FILES}"
    )


    else()
    message(FATAL_ERROR "Unsupported platform ${PLATFORM}")
endif()