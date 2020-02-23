#include "VulkanInitialization.h"
#include "VulkanExtensions.h"

#include <vector>

VkPhysicalDevice CreatePhysicalDevice(const VkInstance& instance);
VkInstance CreateInstance();

VulkanInstanceInfo InitializeVulkan()
{
	VulkanInstanceInfo info = {};
	info.Instance = CreateInstance();
	info.PhysicalDevice = CreatePhysicalDevice(info.Instance);

	return info;
}

static VkPhysicalDevice CreatePhysicalDevice(const VkInstance& instance)
{
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	return physicalDevices[0];
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