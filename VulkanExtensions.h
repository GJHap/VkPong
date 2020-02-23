#pragma once

#include <cstdint>
#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

struct VulkanExtensionInfo
{
	uint32_t ExtensionCount;
	const char** ExtensionNames;
};

VulkanExtensionInfo GetVulkanExtensionInfo();
VkSurfaceKHR CreateVulkanSurface(const VkInstance&, GLFWwindow*);
