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
    "src/Game/Game.cpp"
    "src/Game/Game.hpp"
    "src/Game/Renderer.cpp"
    "src/Game/Renderer.hpp"
    "src/Game/GameObjects/Ball.cpp"
    "src/Game/GameObjects/Ball.hpp"
    "src/Game/GameObjects/BallVertices.hpp"
    "src/Game/GameObjects/GameObject.cpp"
    "src/Game/GameObjects/GameObject.hpp"
    "src/Game/GameObjects/OpponentPaddle.cpp"
    "src/Game/GameObjects/OpponentPaddle.hpp"
    "src/Game/GameObjects/Paddle.cpp"
    "src/Game/GameObjects/Paddle.hpp"
    "src/Game/GameObjects/PlayerPaddle.cpp"
    "src/Game/GameObjects/PlayerPaddle.hpp"
    "src/Structs/Color.hpp"
    "src/Structs/Direction.hpp"
    "src/Structs/Position.hpp"
    "src/Structs/VertexData.cpp"
    "src/Structs/VertexData.hpp"
    "src/Vulkan/BufferInfo.hpp"
    "src/Vulkan/VulkanExtensions.cpp"
    "src/Vulkan/VulkanExtensions.hpp"
    "src/Vulkan/vulkanhpp_include.hpp"
    "src/Vulkan/VulkanState.cpp"
    "src/Vulkan/VulkanState.hpp"
    "src/Vulkan/VulkanObjectCreation/BufferCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/BufferCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/CommandBufferCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/CommandBufferCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/CommandPoolCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/CommandPoolCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/DebugReportbackCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/DebugReportbackCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/FenceCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/FenceCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/FramebufferCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/FramebufferCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/GraphicsPipelineCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/GraphicsPipelineCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/GraphicsPipelineLayoutCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/GraphicsPipelineLayoutCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/ImageViewCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/ImageViewCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/InstanceCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/InstanceCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/LogicalDeviceCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/LogicalDeviceCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/PhysicalDeviceCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/PhysicalDeviceCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/RenderPassCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/RenderPassCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/SemaphoreCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/SemaphoreCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/SurfaceCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/SurfaceCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/SwapchainCreation.cpp"
    "src/Vulkan/VulkanObjectCreation/SwapchainCreation.hpp"
    "src/Vulkan/VulkanObjectCreation/VulkanObjectCreation.hpp"
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
