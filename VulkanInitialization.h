#pragma once

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

struct VulkanInstanceInfo
{
	VkInstance Instance;
	VkPhysicalDevice PhysicalDevice;
	VkSurfaceKHR Surface;
};

VulkanInstanceInfo InitializeVulkan(GLFWwindow*);
