cmake_minimum_required(VERSION 3.15)

project(Pong VERSION 1.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)
add_subdirectory("3rdParty/glfw")

add_executable(Pong Main.cpp)

target_link_libraries(Pong 
                      PRIVATE Vulkan::Vulkan 
                      PUBLIC glfw
                     )
