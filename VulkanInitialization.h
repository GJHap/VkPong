#pragma once

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

struct VulkanInstanceInfo
{
	VkQueue GraphicsQueue;
	VkInstance Instance;
	VkDevice LogicalDevice;
	VkPhysicalDevice PhysicalDevice;
	VkQueue PresentQueue;
	VkSurfaceKHR Surface;
};

VulkanInstanceInfo InitializeVulkan(GLFWwindow*);
