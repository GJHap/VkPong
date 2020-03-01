cmake_minimum_required(VERSION 3.17)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Pong VERSION 1.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)
add_subdirectory("3rdParty/glfw")

FILE(GLOB TOOLS_FILES
    ${SRCROOT}/shaders/fragment/*
)

set(SOURCE_LIST
    "Main.cpp"
    "VulkanInitialization.h"
    "VulkanInitialization.cpp"
    "shaders/fragment/shader.frag"
    "shaders/vertex/shader.vert"
    )

add_executable(Pong ${SOURCE_LIST})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

target_link_libraries(Pong 
                      PRIVATE Vulkan::Vulkan 
                      PUBLIC glfw
                     )
