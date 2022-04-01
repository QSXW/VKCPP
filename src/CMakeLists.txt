cmake_minimum_required(VERSION 3.16)

project("VKCPP_API")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_FILES
    core.cpp
    core.h
    device.cpp
    device.h
    instance.cpp
    instance.h
    IObject.h
    physical_device.cpp
    physical_device.h
    vkcpp.h)

add_library(${PROJECT_NAME} STATIC
    ${PROJECT_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
    vulkan
    $<$<C_COMPILER_ID:GNU>:dl>)
