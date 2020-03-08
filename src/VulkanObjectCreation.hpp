#pragma once

#ifndef VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#endif

#include <vulkan\vulkan.hpp>
#include <GLFW\glfw3.h>

struct QueueInfo
{
	uint32_t queueFamilyIndex;
	vk::Queue queue;
};

struct LogicalDeviceInfo
{
	vk::Device logicalDevice;
	QueueInfo graphicsQueueInfo;
	QueueInfo presentQueueInfo;
};

vk::CommandBuffer createCommandBuffer(const vk::CommandPool&, const vk::Device&, const vk::Extent2D&, const vk::Framebuffer&, const vk::Pipeline&, const vk::RenderPass&);
vk::CommandPool createCommandPool(const vk::Device&, const uint32_t&);
vk::DebugReportCallbackEXT createDebugReportCallback(const vk::Instance&);
vk::Fence createFence(const vk::Device&);
vk::Framebuffer createFramebuffer(const vk::Device&, const vk::ImageView&, const vk::RenderPass&, const vk::Extent2D&);
vk::Pipeline createGraphicsPipeline(const vk::PipelineLayout&, const vk::Device&, const vk::RenderPass&, const vk::Extent2D&);
vk::PipelineLayout createGraphicsPipelineLayout(const vk::Device&);
vk::ImageView createImageView(const vk::Device&, const vk::Image&, const vk::Format&);
vk::Instance createInstance();
LogicalDeviceInfo createLogicalDevice(const vk::PhysicalDevice&, const vk::SurfaceKHR&);
vk::PhysicalDevice createPhysicalDevice(const vk::Instance&);
vk::RenderPass createRenderPass(const vk::Device&, const vk::Format&);
vk::Semaphore createSemaphore(const vk::Device&);
vk::SurfaceKHR createSurface(const vk::Instance&, GLFWwindow*);
vk::SwapchainKHR createSwapchain(const vk::Device&, const vk::PhysicalDevice&, const vk::SurfaceKHR&, const vk::SurfaceFormatKHR&, const vk::SurfaceCapabilitiesKHR&);