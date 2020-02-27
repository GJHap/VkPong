#include <algorithm>
#include <vector>

#include "VulkanInitialization.h"

#include <GLFW\glfw3.h>

const static std::vector<const char*> ENABLED_DEVICE_EXTENSIONS {};

struct VulkanQueueInfo
{
	uint32_t queueFamilyIndex;
	VkQueue queue;
};

struct VulkanLogicalDeviceInfo
{
	VkDevice logicalDevice;
	VulkanQueueInfo graphicsQueueInfo;
	VulkanQueueInfo presentQueueInfo;
};

struct VulkanQueueFamilyIndexInfo
{
	uint32_t graphicsQueueFamilyIndex;
	uint32_t presentQueueFamilyIndex;
};

static VkCommandPool CreateCommandPool(const VkDevice&, const uint32_t&);
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
	info.instance = CreateInstance();
	info.physicalDevice = CreatePhysicalDevice(info.instance);
	info.surface = CreateVulkanSurface(info.instance, glfwWindow);

	VulkanLogicalDeviceInfo logicalDeviceInfo = CreateLogicalDevice(info.physicalDevice, info.surface);
	info.logicalDevice = logicalDeviceInfo.logicalDevice;
	info.graphicsQueue = logicalDeviceInfo.graphicsQueueInfo.queue;
	info.presentQueue = logicalDeviceInfo.presentQueueInfo.queue;

	info.commandPool = CreateCommandPool(info.logicalDevice, logicalDeviceInfo.graphicsQueueInfo.queueFamilyIndex);

	return info;
}

static VkCommandPool CreateCommandPool(const VkDevice& logicalDevice, const uint32_t& queueFamilyIndex)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	VkCommandPool commandPool;
	vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);

	return commandPool;
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

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "VkPong";
	applicationInfo.apiVersion = VK_MAKE_VERSION(12, 2, 132);
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	VkInstance vkInstance;
	vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance);

	return vkInstance;
}

static VulkanLogicalDeviceInfo CreateLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	VulkanQueueFamilyIndexInfo queueInfo = GetVulkanQueueInfo(physicalDevice, surface);
	VkDeviceQueueCreateInfo graphicsQueueCreateInfo = CreateDeviceQueueCreateInfo(queueInfo.graphicsQueueFamilyIndex);
	VkDeviceQueueCreateInfo presentQueueCreateInfo = CreateDeviceQueueCreateInfo(queueInfo.presentQueueFamilyIndex);
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

	VulkanQueueInfo graphicsInfo = { queueInfo.graphicsQueueFamilyIndex, graphicsQueue };
	VulkanQueueInfo presentInfo = { queueInfo.presentQueueFamilyIndex, presentQueue };

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
			queueInfo.graphicsQueueFamilyIndex = i;
		}

		
		if (IsSurfaceSupported(physicalDevice, surface, i))
		{
			queueInfo.presentQueueFamilyIndex = i;
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