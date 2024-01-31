# UniversalUI Graphics CMakeLists.txt

if (CMAKE_SYSTEM_NAME STREQUAL "DARWIN")

elseif (WIN32) 

    find_package(Vulkan REQUIRED) # error

    set(UNIVERSALUI_GLAL_SOURCE
        Source/Vulkan/GLAL.c
    )

    find_package(OpenGL REQUIRED)

    set(PLATFORM_SPECIFIC_GRAPHICS_LIBRARIES
        Vulkan
    )

else()
    message(FATAL_ERROR "Unsupported platform ${CMAKE_SYSTEM_NAME}")
endif()

add_library(UniversalUI-GLAL STATIC ${UNIVERSALUI_GLAL_SOURCE})

target_include_directories(UniversalUI-GLAL PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/
    PUBLIC ${VULKAN_INCLUDE_DIRS}
    )

target_link_libraries(UniversalUI-GLAL PUBLIC ${PLATFORM_SPECIFIC_GRAPHICS_LIBRARIES} ${VULKAN_LIB_LIST})