#pragma once

#include <vulkan\vulkan.h>

struct VulkanInstanceInfo
{
	VkInstance Instance;
};

VulkanInstanceInfo InitializeVulkan();
