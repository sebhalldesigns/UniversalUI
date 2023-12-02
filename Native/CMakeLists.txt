cmake_minimum_required(VERSION 3.5)
project(MyiOSApp)

# Specify the minimum iOS version and architecture
set(CMAKE_OSX_SYSROOT "iphoneos")
set(CMAKE_OSX_ARCHITECTURES "arm64")

set(XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES")
set(XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic")
set(XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "Sebastian Hall")
set(MACOSX_BUNDLE_IDENTIFIER "com.universalui.\${PRODUCT_NAME:identifier}")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.universalui.\${PRODUCT_NAME:identifier}")
set(APP_TYPE MACOSX_BUNDLE)


# Add your source files
add_executable(MyiOSApp main.cpp nApplication.mm 
${CMAKE_SOURCE_DIR}/Include/uApplication.h
${CMAKE_SOURCE_DIR}/Include/nApplication.h
${CMAKE_SOURCE_DIR}/Info.plist)

target_include_directories(MyiOSApp PRIVATE ${CMAKE_SOURCE_DIR}/Include/ )


# Link against required iOS frameworks
target_link_libraries(MyiOSApp
    "-framework UIKit"
    "-framework Foundation"
)



# code signing
set_target_properties(${NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: Sebastian Hall")