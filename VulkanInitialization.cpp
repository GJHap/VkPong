#include "VulkanInitialization.h"
#include "VulkanExtensions.h"

VkInstance CreateInstance();

VulkanInstanceInfo InitializeVulkan()
{
	VulkanInstanceInfo info = {};
	info.Instance = CreateInstance();

	return info;
}

static VkInstance CreateInstance()
{
	VulkanExtensionInfo extensionInfo = GetVulkanExtensionInfo();

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.enabledExtensionCount = extensionInfo.ExtensionCount;
	instanceCreateInfo.ppEnabledExtensionNames = extensionInfo.ExtensionNames;

	VkInstance vkInstance;
	vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance);

	return vkInstance;
}