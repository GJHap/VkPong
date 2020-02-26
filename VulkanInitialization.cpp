#include <algorithm>
#include <vector>

#include "VulkanInitialization.h"

#include <GLFW\glfw3.h>

const static std::vector<const char*> ENABLED_DEVICE_EXTENSIONS {};

struct VulkanQueueInfo
{
	uint32_t FamilyIndex;
	VkQueue Queue;
};

struct VulkanLogicalDeviceInfo
{
	VkDevice LogicalDevice;
	VulkanQueueInfo GraphicsQueueInfo;
	VulkanQueueInfo PresentQueueInfo;
};

struct VulkanQueueFamilyIndexInfo
{
	uint32_t GraphicsFamilyIndex;
	uint32_t PresentFamilyIndex;
};

static VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(const uint32_t&);
static VkInstance CreateInstance();
static VulkanLogicalDeviceInfo CreateLogicalDevice(const VkPhysicalDevice&, const VkSurfaceKHR&);
static VkPhysicalDevice CreatePhysicalDevice(const VkInstance&);
static VkSurfaceKHR CreateVulkanSurface(const VkInstance&, GLFWwindow*);
static std::vector<const char*> GetVulkanInstanceExtensions();
static VulkanQueueFamilyIndexInfo GetVulkanQueueInfo(const VkPhysicalDevice&, const VkSurfaceKHR&);
static bool IsSurfaceSupported(const VkPhysicalDevice&, const VkSurfaceKHR&, const uint32_t&);

VulkanInstanceInfo InitializeVulkan(GLFWwindow* glfwWindow)
{
	VulkanInstanceInfo info = {};
	info.Instance = CreateInstance();
	info.PhysicalDevice = CreatePhysicalDevice(info.Instance);
	info.Surface = CreateVulkanSurface(info.Instance, glfwWindow);

	VulkanLogicalDeviceInfo logicalDeviceInfo = CreateLogicalDevice(info.PhysicalDevice, info.Surface);
	info.LogicalDevice = logicalDeviceInfo.LogicalDevice;
	info.GraphicsQueue = logicalDeviceInfo.GraphicsQueueInfo.Queue;
	info.PresentQueue = logicalDeviceInfo.PresentQueueInfo.Queue;

	return info;
}

static VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(const uint32_t& queueFamilyIndex)
{
	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	return queueCreateInfo;
}

static VkInstance CreateInstance()
{
	std::vector<const char*> instanceExtensions = GetVulkanInstanceExtensions();

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

	VkInstance vkInstance;
	vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance);

	return vkInstance;
}

static VulkanLogicalDeviceInfo CreateLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	VulkanQueueFamilyIndexInfo queueInfo = GetVulkanQueueInfo(physicalDevice, surface);
	VkDeviceQueueCreateInfo graphicsQueueCreateInfo = CreateDeviceQueueCreateInfo(queueInfo.GraphicsFamilyIndex);
	VkDeviceQueueCreateInfo presentQueueCreateInfo = CreateDeviceQueueCreateInfo(queueInfo.PresentFamilyIndex);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = { graphicsQueueCreateInfo, presentQueueCreateInfo };

	VkDeviceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.ppEnabledExtensionNames = ENABLED_DEVICE_EXTENSIONS.data();
	info.enabledExtensionCount = ENABLED_DEVICE_EXTENSIONS.size();
	info.pQueueCreateInfos = queueCreateInfos.data();
	info.queueCreateInfoCount = queueCreateInfos.size();

	VkDevice device;
	vkCreateDevice(physicalDevice, &info, nullptr, &device);

	VkQueue graphicsQueue;
	vkGetDeviceQueue(device, graphicsQueueCreateInfo.queueFamilyIndex, 0, &graphicsQueue);

	VkQueue presentQueue;
	vkGetDeviceQueue(device, presentQueueCreateInfo.queueFamilyIndex, 0, &presentQueue);

	VulkanQueueInfo graphicsInfo = { queueInfo.GraphicsFamilyIndex, graphicsQueue };
	VulkanQueueInfo presentInfo = { queueInfo.PresentFamilyIndex, presentQueue };

	return { device, graphicsInfo, presentInfo };
}

static VkPhysicalDevice CreatePhysicalDevice(const VkInstance& instance)
{
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	return physicalDevices[0];
}

static VkSurfaceKHR CreateVulkanSurface(const VkInstance& vkInstance, GLFWwindow* glfwWindow)
{
	VkSurfaceKHR vkSurface;
	glfwCreateWindowSurface(vkInstance, glfwWindow, nullptr, &vkSurface);

	return vkSurface;
}

static std::vector<const char*> GetVulkanInstanceExtensions()
{
	uint32_t extensionCount;
	const char** ppExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	std::vector<const char*> extensions(extensionCount);
	std::copy(ppExtensions, ppExtensions + extensionCount, extensions.begin());

	return extensions;
}

static VulkanQueueFamilyIndexInfo GetVulkanQueueInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	uint32_t queueFamilyPropertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

	VulkanQueueFamilyIndexInfo queueInfo = {};

	for (auto i = 0; i < queueFamilyProperties.size(); ++i)
	{
		if ((queueFamilyProperties[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) == VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
		{
			queueInfo.GraphicsFamilyIndex = i;
		}

		
		if (IsSurfaceSupported(physicalDevice, surface, i))
		{
			queueInfo.PresentFamilyIndex = i;
		}
	}

	return queueInfo;
}

static bool IsSurfaceSupported(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const uint32_t& queueFamilyIndex)
{
	VkBool32 surfaceSupported;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &surfaceSupported);

	return surfaceSupported == VK_TRUE;
}