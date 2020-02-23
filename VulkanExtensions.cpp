#include "VulkanExtensions.h"
#include <GLFW\glfw3.h>

VulkanExtensionInfo GetVulkanExtensionInfo()
{
	uint32_t extensionCount;
	const char** extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);

	return { extensionCount, extensionNames };
}
