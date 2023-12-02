# UniversalUI Core CMakeLists.txt

# List all your source files explicitly
set(UNIVERSALUI_CORE_GENERIC_SOURCE
    Source/uApplication.cpp
    Source/UniversalUI.cpp
)

add_library(UniversalUI-Core STATIC ${UNIVERSALUI_CORE_GENERIC_SOURCE})

target_include_directories(UniversalUI-Core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../Native/Include/
)

target_link_libraries(UniversalUI-Core PRIVATE UniversalUI-Native)