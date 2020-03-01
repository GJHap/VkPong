#include "VulkanInitialization.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include <GLFW\glfw3.h>

#ifdef NDEBUG
const bool enableValidation = false;
#else
const bool enableValidation = true;
#endif

const static std::vector<const char*> ENABLED_DEVICE_EXTENSIONS{};

const static std::vector<const char*> INSTANCE_VALIDATION_LAYERS
{
	"VK_LAYER_LUNARG_standard_validation"
};

const static std::vector<const char*> INSTANCE_EXTENSIONS
{
	"VK_EXT_debug_report"
};

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

static VkCommandBuffer createCommandBuffer(const VkCommandPool&, const VkDevice&);
static VkCommandPool createCommandPool(const VkDevice&, const uint32_t&);
static VkDebugReportCallbackEXT createDebugReportCallback(const VkInstance&);
static VkDeviceQueueCreateInfo createDeviceQueueCreateInfo(const uint32_t&);
static VkInstance createInstance();
static VulkanLogicalDeviceInfo createLogicalDevice(const VkPhysicalDevice&, const VkSurfaceKHR&);
static VkPhysicalDevice createPhysicalDevice(const VkInstance&);
static VkRenderPass createRenderPass(const VkDevice&);
static VkSurfaceKHR createVulkanSurface(const VkInstance&, GLFWwindow*);
static VkBool32 debugCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char*, void*);
static std::vector<const char*> getVulkanInstanceExtensions();
static VulkanQueueFamilyIndexInfo getVulkanQueueInfo(const VkPhysicalDevice&, const VkSurfaceKHR&);
static void handleError(VkResult, const std::string&);
static bool isSurfaceSupported(const VkPhysicalDevice&, const VkSurfaceKHR&, const uint32_t&);
static void vulkanDestroyDebugReportCallbackEXT(const VkInstance&, VkDebugReportCallbackEXT, const VkAllocationCallbacks*);
static VkResult vulkanCreateDebugReportCallbackEXT(const VkInstance&, const VkDebugReportCallbackCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugReportCallbackEXT*);

VulkanInstanceInfo initializeVulkan(GLFWwindow* glfwWindow)
{
	VulkanInstanceInfo info = {};
	info.instance = createInstance();

	if (enableValidation)
	{
		info.debugReportCallback = createDebugReportCallback(info.instance);
	}

	info.physicalDevice = createPhysicalDevice(info.instance);
	info.surface = createVulkanSurface(info.instance, glfwWindow);

	VulkanLogicalDeviceInfo logicalDeviceInfo = createLogicalDevice(info.physicalDevice, info.surface);
	info.logicalDevice = logicalDeviceInfo.logicalDevice;
	info.graphicsQueue = logicalDeviceInfo.graphicsQueueInfo.queue;
	info.presentQueue = logicalDeviceInfo.presentQueueInfo.queue;

	info.commandPool = createCommandPool(info.logicalDevice, logicalDeviceInfo.graphicsQueueInfo.queueFamilyIndex);
	info.commandBuffer = createCommandBuffer(info.commandPool, info.logicalDevice);
	info.renderPass = createRenderPass(info.logicalDevice);

	return info;
}

static VkCommandBuffer createCommandBuffer(const VkCommandPool& commandPool, const VkDevice& logicalDevice)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandBufferCount = 1;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkCommandBuffer commandBuffer;
	handleError(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &commandBuffer), "Failed to allocate command buffer.");

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	handleError(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo), "Failed to begin recording command buffer.");

	handleError(vkEndCommandBuffer(commandBuffer), "Failed to finish recording command buffer.");

	return commandBuffer;
}

static VkCommandPool createCommandPool(const VkDevice& logicalDevice, const uint32_t& queueFamilyIndex)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	VkCommandPool commandPool;
	handleError(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool), "Failed to create command pool.");

	return commandPool;
}

static VkDebugReportCallbackEXT createDebugReportCallback(const VkInstance& instance)
{
	VkDebugReportCallbackCreateInfoEXT callbackCreateInfo;
	callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	callbackCreateInfo.pNext = nullptr;
	callbackCreateInfo.flags
		= VK_DEBUG_REPORT_ERROR_BIT_EXT
		| VK_DEBUG_REPORT_WARNING_BIT_EXT
		| VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
		| VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	callbackCreateInfo.pfnCallback = &debugCallback;
	callbackCreateInfo.pUserData = nullptr;

	VkDebugReportCallbackEXT callback;
	handleError(vulkanCreateDebugReportCallbackEXT(instance, &callbackCreateInfo, nullptr, &callback), "Failed to create debug callback.");

	return callback;
}

static VkDeviceQueueCreateInfo createDeviceQueueCreateInfo(const uint32_t& queueFamilyIndex)
{
	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	return queueCreateInfo;
}

static VkInstance createInstance()
{
	std::vector<const char*> instanceExtensions = getVulkanInstanceExtensions();
	std::vector<const char*> enabledLayers;

	if (enableValidation)
	{
		enabledLayers = INSTANCE_VALIDATION_LAYERS;
		std::copy(INSTANCE_EXTENSIONS.begin(), INSTANCE_EXTENSIONS.end(), std::back_inserter(instanceExtensions));
	}

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "VkPong";
	applicationInfo.apiVersion = VK_MAKE_VERSION(12, 2, 132);
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
	instanceCreateInfo.enabledLayerCount = enabledLayers.size();
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	VkInstance vkInstance;
	handleError(vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance), "Failed to create instance.");

	return vkInstance;
}

static VulkanLogicalDeviceInfo createLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	VulkanQueueFamilyIndexInfo queueInfo = getVulkanQueueInfo(physicalDevice, surface);
	VkDeviceQueueCreateInfo graphicsQueueCreateInfo = createDeviceQueueCreateInfo(queueInfo.graphicsQueueFamilyIndex);
	VkDeviceQueueCreateInfo presentQueueCreateInfo = createDeviceQueueCreateInfo(queueInfo.presentQueueFamilyIndex);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = { graphicsQueueCreateInfo, presentQueueCreateInfo };

	VkDeviceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.ppEnabledExtensionNames = ENABLED_DEVICE_EXTENSIONS.data();
	info.enabledExtensionCount = ENABLED_DEVICE_EXTENSIONS.size();
	info.pQueueCreateInfos = queueCreateInfos.data();
	info.queueCreateInfoCount = queueCreateInfos.size();

	VkDevice device;
	handleError(vkCreateDevice(physicalDevice, &info, nullptr, &device), "Failed to create logical device.");

	VkQueue graphicsQueue;
	vkGetDeviceQueue(device, graphicsQueueCreateInfo.queueFamilyIndex, 0, &graphicsQueue);

	VkQueue presentQueue;
	vkGetDeviceQueue(device, presentQueueCreateInfo.queueFamilyIndex, 0, &presentQueue);

	VulkanQueueInfo graphicsInfo = { queueInfo.graphicsQueueFamilyIndex, graphicsQueue };
	VulkanQueueInfo presentInfo = { queueInfo.presentQueueFamilyIndex, presentQueue };

	return { device, graphicsInfo, presentInfo };
}

static VkRenderPass createRenderPass(const VkDevice& logicalDevice)
{
	VkAttachmentDescription attachmentDescription;
	attachmentDescription.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDescription.flags = VkAttachmentDescriptionFlagBits::VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
	attachmentDescription.format = VkFormat::VK_FORMAT_R8_UNORM;
	attachmentDescription.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDescription.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
	attachmentDescription.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;

	VkAttachmentReference attachmentReference = {};
	attachmentReference.attachment = 0;
	attachmentReference.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.colorAttachmentCount = 2;
	subpassDescription.pColorAttachments = &attachmentReference;
	subpassDescription.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDescription;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	VkRenderPass renderPass;
	handleError(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &renderPass), "Failed to create render pass.");

	return renderPass;
}

static VkPhysicalDevice createPhysicalDevice(const VkInstance& instance)
{
	uint32_t physicalDeviceCount;
	handleError(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr), "Failed to get physical device.");

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	handleError(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()), "Failed to get physical device.");

	return physicalDevices[0];
}

static VkSurfaceKHR createVulkanSurface(const VkInstance& vkInstance, GLFWwindow* glfwWindow)
{
	VkSurfaceKHR vkSurface;
	handleError(glfwCreateWindowSurface(vkInstance, glfwWindow, nullptr, &vkSurface), "Failed to create surface.");

	return vkSurface;
}

static VkBool32 debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	std::cerr << pMessage << "\n\n";
	return VK_FALSE;
}

static std::vector<const char*> getVulkanInstanceExtensions()
{
	uint32_t extensionCount;
	const char** ppExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	std::vector<const char*> extensions(extensionCount);
	std::copy(ppExtensions, ppExtensions + extensionCount, extensions.begin());

	return extensions;
}

static VulkanQueueFamilyIndexInfo getVulkanQueueInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
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

		if (isSurfaceSupported(physicalDevice, surface, i))
		{
			queueInfo.presentQueueFamilyIndex = i;
		}
	}

	return queueInfo;
}

static void handleError(VkResult result, const std::string& errorMessage)
{
	std::stringstream ss;
	switch (result)
	{
	case VkResult::VK_SUCCESS:
		break;
	case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT:
		ss << errorMessage << " Extension not present.";
		throw std::runtime_error(ss.str());
	default:
		ss << errorMessage;
		throw std::runtime_error(ss.str());
	}
}

static bool isSurfaceSupported(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const uint32_t& queueFamilyIndex)
{
	VkBool32 surfaceSupported;
	handleError(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &surfaceSupported), "Failed to determine surface support.");

	return surfaceSupported == VK_TRUE;
}

static void vulkanDestroyDebugReportCallbackEXT(const VkInstance& instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT =
		reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
		(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));

	vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

static VkResult vulkanCreateDebugReportCallbackEXT(const VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
	return vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}