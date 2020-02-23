#include "VulkanExtensions.h"
#include <GLFW\glfw3.h>

VulkanExtensionInfo GetVulkanExtensionInfo()
{
	uint32_t extensionCount;
	const char** extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);

	return { extensionCount, extensionNames };
}

VkSurfaceKHR CreateVulkanSurface(const VkInstance& vkInstance, GLFWwindow* glfwWindow)
{
	VkSurfaceKHR vkSurface;
	glfwCreateWindowSurface(vkInstance, glfwWindow, nullptr, &vkSurface);

	return vkSurface;
}
