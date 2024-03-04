set(UNIVERSALUI_CORE_SOURCE
    src/Types/uWindow.cpp
)

add_library(UniversalUI-Core STATIC ${UNIVERSALUI_CORE_SOURCE})
target_include_directories(UniversalUI-Core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/types
    ${CMAKE_SOURCE_DIR}/../GSP/src/GSP-System/include  
    ${CMAKE_SOURCE_DIR}/../GSP/src/GSP-Graphics/include
)