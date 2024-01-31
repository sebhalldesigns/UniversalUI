# linux dependencies libgl1-mesa-dev libx11-dev

cmake_minimum_required(VERSION 3.5)
project(UniversalUI)
add_definitions(-w)

if (EMSCRIPTEN)

    message("Configuring UniversalUI for Web...")

elseif (PLATFORM STREQUAL "ios")
    
    message("Configuring UniversalUI for iOS...")

    # Specify the minimum iOS version and architecture
    set(CMAKE_SYSTEM_NAME iOS)  
    set(CMAKE_OSX_SYSROOT "iphoneos")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")

    add_subdirectory(Core)

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

elseif(PLATFORM STREQUAL "macos")

    message("Configuring UniversalUI for macOS...")

    set(CMAKE_SYSTEM_NAME MacOSX)  
    set(CMAKE_OSX_ARCHITECTURES "arm64")

    add_subdirectory(Core)

    add_executable(UniversalUI MACOSX_BUNDLE main.cpp)
    target_link_libraries(UniversalUI PRIVATE UniversalUI-Core)

    set(MACOS_RESOURCE_FILES 
        External/Config/macOS/Info.plist
        External/Config/macOS/Assets.xcassets
        External/Config/macOS/Base.lproj
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
        RESOURCE "${MACOS_RESOURCE_FILES}"
    )

elseif(WIN32)

message("Configuring UniversalUI for Win32...")


    add_subdirectory(GLAL)
    add_subdirectory(Graphics)
    add_subdirectory(Core)

    add_executable(UniversalUI main.cpp)
    target_link_libraries(UniversalUI PRIVATE UniversalUI-Core)

elseif(NOT DEFINED PLATFORM)
    message(FATAL_ERROR "Platform not defined! Please specify a platform with -DPLATFORM=")
else()
    message(FATAL_ERROR "Unsupported platform ${PLATFORM}")
endif()