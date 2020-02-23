#include "VulkanInitialization.h"
#include "VulkanExtensions.h"

#include <vector>

static VkInstance CreateInstance();
static VkPhysicalDevice CreatePhysicalDevice(const VkInstance&);

VulkanInstanceInfo InitializeVulkan(GLFWwindow* glfwWindow)
{
	VulkanInstanceInfo info = {};
	info.Instance = CreateInstance();
	info.PhysicalDevice = CreatePhysicalDevice(info.Instance);
	info.Surface = CreateVulkanSurface(info.Instance, glfwWindow);

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

static VkPhysicalDevice CreatePhysicalDevice(const VkInstance& instance)
{
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	return physicalDevices[0];
}
