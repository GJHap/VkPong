#pragma once

#include <vulkan\vulkan.h>

struct VulkanInstanceInfo
{
	VkPhysicalDevice PhysicalDevice;
	VkInstance Instance;
};

VulkanInstanceInfo InitializeVulkan();
