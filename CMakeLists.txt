cmake_minimum_required(VERSION 3.17)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Pong VERSION 1.0)



set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)
add_subdirectory("3rdParty/glfw")

set(SOURCE_LIST
    ".gitignore"
    ".gitmodules"
    "README.md"
    "LICENSE"
    "src/Main.cpp"
    "src/VulkanState.hpp"
    "src/VulkanState.cpp"
    "src/VulkanObjectCreation.hpp"
    "src/VulkanObjectCreation.cpp"
    "src/VulkanExtensions.hpp"
    "src/VulkanExtensions.cpp"
    "src/vulkanhpp_include.hpp"
    "shaders/fragment/shader.frag"
    "shaders/vertex/shader.vert"
    )

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

add_custom_command(TARGET ${PROJECT_NAME}
                   PRE_BUILD
                   COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex/shader.vert -o ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex/shader.spv
                   COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment/shader.frag -o ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment/shader.spv            
)

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

target_link_libraries(${PROJECT_NAME} 
                      PRIVATE Vulkan::Vulkan 
                      PUBLIC glfw
                     )
