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
    "src/BufferInfo.hpp"
    "src/VertexData.hpp"
    "src/Main.cpp"
    "src/VulkanState.cpp"
    "src/VulkanState.hpp"
    "src/VulkanExtensions.cpp"
    "src/VulkanExtensions.hpp"
    "src/vulkanhpp_include.hpp"
    "src/VulkanObjectCreation/BufferCreation.hpp"
    "src/VulkanObjectCreation/BufferCreation.cpp"
    "src/VulkanObjectCreation/CommandBufferCreation.cpp"
    "src/VulkanObjectCreation/CommandBufferCreation.hpp"
    "src/VulkanObjectCreation/CommandPoolCreation.cpp"
    "src/VulkanObjectCreation/CommandPoolCreation.hpp"
    "src/VulkanObjectCreation/DebugReportbackCreation.cpp"
    "src/VulkanObjectCreation/DebugReportbackCreation.hpp"
    "src/VulkanObjectCreation/FenceCreation.cpp"
    "src/VulkanObjectCreation/FenceCreation.hpp"
    "src/VulkanObjectCreation/FramebufferCreation.cpp"
    "src/VulkanObjectCreation/FramebufferCreation.hpp"
    "src/VulkanObjectCreation/GraphicsPipelineCreation.cpp"
    "src/VulkanObjectCreation/GraphicsPipelineCreation.hpp"
    "src/VulkanObjectCreation/GraphicsPipelineLayoutCreation.cpp"
    "src/VulkanObjectCreation/GraphicsPipelineLayoutCreation.hpp"
    "src/VulkanObjectCreation/ImageViewCreation.cpp"
    "src/VulkanObjectCreation/ImageViewCreation.hpp"
    "src/VulkanObjectCreation/InstanceCreation.cpp"
    "src/VulkanObjectCreation/InstanceCreation.hpp"
    "src/VulkanObjectCreation/LogicalDeviceCreation.cpp"
    "src/VulkanObjectCreation/LogicalDeviceCreation.hpp"
    "src/VulkanObjectCreation/PhysicalDeviceCreation.cpp"
    "src/VulkanObjectCreation/PhysicalDeviceCreation.hpp"
    "src/VulkanObjectCreation/RenderPassCreation.cpp"
    "src/VulkanObjectCreation/RenderPassCreation.hpp"
    "src/VulkanObjectCreation/SemaphoreCreation.cpp"
    "src/VulkanObjectCreation/SemaphoreCreation.hpp"
    "src/VulkanObjectCreation/SurfaceCreation.cpp"
    "src/VulkanObjectCreation/SurfaceCreation.hpp"
    "src/VulkanObjectCreation/SwapchainCreation.cpp"
    "src/VulkanObjectCreation/SwapchainCreation.hpp"
    "src/VulkanObjectCreation/VulkanObjectCreation.hpp"
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
