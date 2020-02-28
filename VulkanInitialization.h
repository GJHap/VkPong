#pragma once

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

struct VulkanInstanceInfo
{
	VkCommandBuffer commandBuffer;
	VkCommandPool commandPool;
	VkQueue graphicsQueue;
	VkInstance instance;
	VkDevice logicalDevice;
	VkPhysicalDevice physicalDevice;
	VkQueue presentQueue;
	VkRenderPass renderPass;
	VkSurfaceKHR surface;
};

VulkanInstanceInfo initializeVulkan(GLFWwindow*);
