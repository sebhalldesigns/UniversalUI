cmake_minimum_required(VERSION 3.1...3.28)

project(universalui)

add_executable(universalui main.cpp)

target_include_directories(universalui PUBLIC ${CMAKE_SOURCE_DIR}/../../universalui/universalui_api/bindings/ccp/include/UniversalUI)
set(UNIVERSALUI_DLL_PATH "${CMAKE_SOURCE_DIR}/../../universalui/target/debug/libUniversalUI.dll")
target_link_libraries(universalui ${CMAKE_SOURCE_DIR}/../../universalui/target/debug/libUniversalUI.lib ws2_32.lib kernel32.lib user32.lib ntdll.lib userenv.lib bcrypt.lib)

# Copy the DLL to the build directory after building MyApp
#add_custom_command(TARGET universalui POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                           ${UNIVERSALUI_DLL_PATH}
#                           $<TARGET_FILE_DIR:universalui>)