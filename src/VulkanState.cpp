#include "VulkanState.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

#include <GLFW\glfw3.h>

#ifdef NDEBUG
const bool enableValidation = false;
#else
const bool enableValidation = true;
#endif

const static std::vector<const char*> DEVICE_EXTENSIONS
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const static std::vector<const char*> INSTANCE_VALIDATION_LAYERS
{
	"VK_LAYER_LUNARG_standard_validation"
};

const static std::vector<const char*> INSTANCE_EXTENSIONS
{
	"VK_EXT_debug_report"
};

struct QueueInfo
{
	uint32_t queueFamilyIndex;
	VkQueue queue;
};

struct LogicalDeviceInfo
{
	VkDevice logicalDevice;
	QueueInfo graphicsQueueInfo;
	QueueInfo presentQueueInfo;
};

struct QueueFamilyIndexInfo
{
	uint32_t graphicsQueueFamilyIndex;
	uint32_t presentQueueFamilyIndex;
};

static VkApplicationInfo createApplicationInfo();
static VkAttachmentDescription createAttachmentDescription(const VkFormat&);
static VkAttachmentReference createAttachmentReference();
static VkCommandBuffer createCommandBuffer(const VkCommandPool&, const VkDevice&, const VkExtent2D&, const VkFramebuffer&, const VkPipeline&, const VkRenderPass&);
static VkCommandPool createCommandPool(const VkDevice&, const uint32_t&);
static VkComponentMapping createComponentMapping();
static VkDebugReportCallbackEXT createDebugReportCallback(const VkInstance&);
static VkDeviceQueueCreateInfo createDeviceQueueCreateInfo(const uint32_t&, const float&);
static VkFence createFence(const VkDevice&);
static VkFramebuffer createFramebuffer(const VkDevice&, const VkImageView&, const VkRenderPass&, const VkExtent2D&);
static VkPipeline createGraphicsPipeline(const VkPipelineLayout&, const VkDevice&, const VkRenderPass&, const VkExtent2D&);
static VkPipelineLayout createGraphicsPipelineLayout(const VkDevice&);
static VkInstance createInstance();
static VkImageSubresourceRange createImageSubresourceRange();
static VkImageView createImageView(const VkDevice&, const VkImage&, const VkFormat&);
static LogicalDeviceInfo createLogicalDevice(const VkPhysicalDevice&, const VkSurfaceKHR&);
static VkPhysicalDevice createPhysicalDevice(const VkInstance&);
static VkPipelineRasterizationStateCreateInfo createPipelineRasterizationStateCreateInfo();
static VkPipelineInputAssemblyStateCreateInfo createPipelineInputAssemblyStateCreateInfo();
static VkRenderPass createRenderPass(const VkDevice&, const VkFormat&);
static VkSemaphore createSemaphore(const VkDevice&);
static VkShaderModule createShaderModule(const VkDevice&, const std::string&);
static VkPipelineShaderStageCreateInfo createShaderStageCreateInfo(const VkDevice&, const VkShaderStageFlagBits&, const std::string&);
static VkSubpassDescription createSubpassDescription(const VkAttachmentReference&);
static VkSurfaceKHR createSurface(const VkInstance&, GLFWwindow*);
static VkSwapchainKHR createSwapchain(const VkDevice&, const VkPhysicalDevice&, const VkSurfaceKHR&, const VkSurfaceFormatKHR&, const VkSurfaceCapabilitiesKHR&);
static VkPipelineVertexInputStateCreateInfo createVertexInputStateCreateInfo();
static VkBool32 debugCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char*, void*);
static std::vector<VkDeviceQueueCreateInfo> getDeviceCreateInfos(const VkDeviceQueueCreateInfo&, const VkDeviceQueueCreateInfo&);
static std::vector<char> getShaderCode(const std::string&);
static std::vector<VkImage> getSwapchainImages(const VkDevice&, const VkSwapchainKHR&);
static std::vector<const char*> getVulkanInstanceExtensions();
static QueueFamilyIndexInfo getVulkanQueueInfo(const VkPhysicalDevice&, const VkSurfaceKHR&);
static void handleError(VkResult, const std::string&);
static bool isSurfaceSupported(const VkPhysicalDevice&, const VkSurfaceKHR&, const uint32_t&);
static void recordCommandBuffer(const VkCommandBuffer&, const VkExtent2D&, const VkFramebuffer&, const VkPipeline&, const VkRenderPass&);
static void vulkanDestroyDebugReportCallbackEXT(const VkInstance&, VkDebugReportCallbackEXT, const VkAllocationCallbacks*);
static VkResult vulkanCreateDebugReportCallbackEXT(const VkInstance&, const VkDebugReportCallbackCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugReportCallbackEXT*);

VulkanState::VulkanState(GLFWwindow* glfwWindow)
{
	m_instance = createInstance();

	if (enableValidation)
	{
		m_debugReportCallback = createDebugReportCallback(m_instance);
	}

	m_physicalDevice = createPhysicalDevice(m_instance);
	m_surface = createSurface(m_instance, glfwWindow);
	LogicalDeviceInfo logicalDeviceInfo = createLogicalDevice(m_physicalDevice, m_surface);
	m_logicalDevice = logicalDeviceInfo.logicalDevice;
	m_graphicsQueue = logicalDeviceInfo.graphicsQueueInfo.queue;
	m_presentQueue = logicalDeviceInfo.presentQueueInfo.queue;

	uint32_t surfaceFormatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatCount, surfaceFormats.data());
	VkSurfaceFormatKHR surfaceFormat = surfaceFormats[0];

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	handleError(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities), "Failed to get surface capabilities.");

	m_renderPass = createRenderPass(m_logicalDevice, surfaceFormat.format);
	m_graphicsPipelineLayout = createGraphicsPipelineLayout(m_logicalDevice);
	m_graphicsPipeline = createGraphicsPipeline(m_graphicsPipelineLayout, m_logicalDevice, m_renderPass, surfaceCapabilities.currentExtent);
	m_swapChain = createSwapchain(m_logicalDevice, m_physicalDevice, m_surface, surfaceFormat, surfaceCapabilities);
	m_swapchainImages = getSwapchainImages(m_logicalDevice, m_swapChain);
	std::transform(m_swapchainImages.cbegin(), m_swapchainImages.cend(), std::back_inserter(m_swapchainImageViews), [this, surfaceFormat](VkImage swapchainImage)
	{
		return createImageView(m_logicalDevice, swapchainImage, surfaceFormat.format);
	});
	std::transform(m_swapchainImageViews.cbegin(), m_swapchainImageViews.cend(), std::back_inserter(m_swapchainFramebuffers), [this, surfaceCapabilities](VkImageView swapchainImageView)
	{
		return createFramebuffer(m_logicalDevice, swapchainImageView, m_renderPass, surfaceCapabilities.currentExtent);
	});

	m_commandPool = createCommandPool(m_logicalDevice, logicalDeviceInfo.graphicsQueueInfo.queueFamilyIndex);
	std::transform(m_swapchainFramebuffers.cbegin(), m_swapchainFramebuffers.cend(), std::back_inserter(m_commandBuffers), [this, surfaceCapabilities](const VkFramebuffer& framebuffer)
	{
		return createCommandBuffer(m_commandPool, m_logicalDevice, surfaceCapabilities.currentExtent, framebuffer, m_graphicsPipeline, m_renderPass);
	});
	std::generate_n(std::back_inserter(m_imageAvailableSemaphores), m_swapchainFramebuffers.size(), [this]() { return createSemaphore(m_logicalDevice); });
	std::generate_n(std::back_inserter(m_imageRenderedSemaphores), m_swapchainFramebuffers.size(), [this]() { return createSemaphore(m_logicalDevice); });
	std::generate_n(std::back_inserter(m_fences), m_swapchainFramebuffers.size(), [this]() { return createFence(m_logicalDevice); });
}

VulkanState::~VulkanState()
{
	vkDeviceWaitIdle(m_logicalDevice);
	for (const VkFence& fence : m_fences) vkDestroyFence(m_logicalDevice, fence, nullptr);
	for (const VkSemaphore& semaphore : m_imageRenderedSemaphores) vkDestroySemaphore(m_logicalDevice, semaphore, nullptr);
	for (const VkSemaphore& semaphore : m_imageAvailableSemaphores) vkDestroySemaphore(m_logicalDevice, semaphore, nullptr);
	for (const VkCommandBuffer& commandBuffer : m_commandBuffers) vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &commandBuffer);
	vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
	for (const VkFramebuffer& swapchainFramebuffer : m_swapchainFramebuffers) vkDestroyFramebuffer(m_logicalDevice, swapchainFramebuffer, nullptr);
	for (const VkImageView& swapchainImageView : m_swapchainImageViews) vkDestroyImageView(m_logicalDevice, swapchainImageView, nullptr);
	vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
	vkDestroyPipeline(m_logicalDevice, m_graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(m_logicalDevice, m_graphicsPipelineLayout, nullptr);
	vkDestroyRenderPass(m_logicalDevice, m_renderPass, nullptr);
	vkDestroyDevice(m_logicalDevice, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vulkanDestroyDebugReportCallbackEXT(m_instance, m_debugReportCallback, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

VkCommandBuffer VulkanState::commandBuffer(const uint32_t& idx) const
{
	return m_commandBuffers[idx];
}

VkFence VulkanState::fence(const uint32_t& idx) const
{
	return m_fences[idx];
}

VkQueue VulkanState::graphicsQueue() const
{
	return m_graphicsQueue;
}

VkSemaphore VulkanState::imageAvailableSemaphore(const uint32_t& idx) const
{
	return m_imageAvailableSemaphores[idx];
}

VkSemaphore VulkanState::imageRenderedSemaphore(const uint32_t& idx) const
{
	return m_imageRenderedSemaphores[idx];
}

VkDevice VulkanState::logicalDevice() const
{
	return m_logicalDevice;
}

VkQueue VulkanState::presentQueue() const
{
	return m_presentQueue;
}

VkSwapchainKHR VulkanState::swapchain() const
{
	return m_swapChain;
}

uint32_t VulkanState::swapchainImageCount() const
{
	return m_swapchainImages.size();
}

static VkApplicationInfo createApplicationInfo()
{
	VkApplicationInfo applicationInfo = {};
	applicationInfo.apiVersion = VK_MAKE_VERSION(12, 2, 132);
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.engineVersion = 0;
	applicationInfo.pApplicationName = "VkPong";
	applicationInfo.pNext = nullptr;
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

	return applicationInfo;
}

static VkAttachmentDescription createAttachmentDescription(const VkFormat& format)
{
	VkAttachmentDescription attachmentDescription;
	attachmentDescription.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachmentDescription.flags = 0;
	attachmentDescription.format = format;
	attachmentDescription.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;

	return attachmentDescription;
}

static VkAttachmentReference createAttachmentReference()
{
	VkAttachmentReference attachmentReference = {};
	attachmentReference.attachment = 0;
	attachmentReference.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	return attachmentReference;
}

static VkCommandBuffer createCommandBuffer(const VkCommandPool& commandPool, const VkDevice& logicalDevice, const VkExtent2D& extent, const VkFramebuffer& framebuffer, const VkPipeline& graphicsPipeline, const VkRenderPass& renderPass)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.commandBufferCount = 1;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

	VkCommandBuffer commandBuffer;
	handleError(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &commandBuffer), "Failed to allocate command buffer.");

	recordCommandBuffer(commandBuffer, extent, framebuffer, graphicsPipeline, renderPass);

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

static VkComponentMapping createComponentMapping()
{
	VkComponentMapping componentMapping = {};
	componentMapping.a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
	componentMapping.b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
	componentMapping.g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
	componentMapping.r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;

	return componentMapping;
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

static VkDeviceQueueCreateInfo createDeviceQueueCreateInfo(const uint32_t& queueFamilyIndex, const float& queuePriority)
{
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.flags = 0;
	queueCreateInfo.pNext = nullptr;
	queueCreateInfo.pQueuePriorities = &queuePriority;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

	return queueCreateInfo;
}

static VkFence createFence(const VkDevice& logicalDevice)
{
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.flags = VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	VkFence fence;
	vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &fence);

	return fence;
}

static VkFramebuffer createFramebuffer(const VkDevice& logicalDevice, const VkImageView& imageView, const VkRenderPass& renderPass, const VkExtent2D& extent)
{
	VkFramebufferCreateInfo framebufferCreateInfo = {};
	framebufferCreateInfo.attachmentCount = 1;
	framebufferCreateInfo.flags = 0;
	framebufferCreateInfo.height = extent.height;
	framebufferCreateInfo.layers = 1;
	framebufferCreateInfo.pAttachments = &imageView;
	framebufferCreateInfo.pNext = nullptr;
	framebufferCreateInfo.renderPass = renderPass;
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.width = extent.width;

	VkFramebuffer framebuffer;
	handleError(vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &framebuffer), "Failed to create frame buffer.");

	return framebuffer;
}

static VkPipeline createGraphicsPipeline(const VkPipelineLayout& graphicsPipelineLayout, const VkDevice& logicalDevice, const VkRenderPass& renderPass, const VkExtent2D& extent)
{
	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = createPipelineInputAssemblyStateCreateInfo();
	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = createPipelineRasterizationStateCreateInfo();
	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = createVertexInputStateCreateInfo();
	std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos
	{
		createShaderStageCreateInfo(logicalDevice, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, "./shaders/vertex/shader.spv"),
		createShaderStageCreateInfo(logicalDevice, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, "./shaders/fragment/shader.spv")
	};

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkRect2D scissors = {};
	scissors.extent = extent;
	scissors.offset = { 0, 0 };
	VkViewport viewport;
	viewport.height = extent.height;
	viewport.maxDepth = 1.0f;
	viewport.minDepth = 0.0f;
	viewport.width = extent.width;
	viewport.x = 0;
	viewport.y = 0;
	VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};
	pipelineViewportStateCreateInfo.flags = 0;
	pipelineViewportStateCreateInfo.pNext = nullptr;
	pipelineViewportStateCreateInfo.pScissors = &scissors;
	pipelineViewportStateCreateInfo.pViewports = &viewport;
	pipelineViewportStateCreateInfo.scissorCount = 1;
	pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pipelineViewportStateCreateInfo.viewportCount = 1;

	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};
	pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
		| VK_COLOR_COMPONENT_G_BIT
		| VK_COLOR_COMPONENT_B_BIT
		| VK_COLOR_COMPONENT_A_BIT;
	pipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
	pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
	pipelineColorBlendStateCreateInfo.attachmentCount = 1;
	pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;
	pipelineColorBlendStateCreateInfo.flags = 0;
	pipelineColorBlendStateCreateInfo.logicOp = VkLogicOp::VK_LOGIC_OP_COPY;
	pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
	pipelineColorBlendStateCreateInfo.pNext = nullptr;
	pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = 0;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.layout = graphicsPipelineLayout;
	graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
	graphicsPipelineCreateInfo.pDynamicState = nullptr;
	graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampling;
	graphicsPipelineCreateInfo.pNext = nullptr;
	graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pStages = shaderCreateInfos.data();
	graphicsPipelineCreateInfo.pTessellationState = nullptr;
	graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.stageCount = shaderCreateInfos.size();
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.subpass = 0;

	VkPipeline graphicsPipeline;
	handleError(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &graphicsPipeline), "Failed to create graphics pipeline.");

	for (VkPipelineShaderStageCreateInfo shaderCreateInfo : shaderCreateInfos)
	{
		vkDestroyShaderModule(logicalDevice, shaderCreateInfo.module, nullptr);
	}

	return graphicsPipeline;
}

static VkPipelineLayout createGraphicsPipelineLayout(const VkDevice& logicalDevice)
{
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.pNext = nullptr;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
	pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 0;
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	VkPipelineLayout pipelineLayout;
	handleError(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout), "Failed to create graphics pipeline layout.");

	return pipelineLayout;
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

	VkApplicationInfo applicationInfo = createApplicationInfo();

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

static VkImageSubresourceRange createImageSubresourceRange()
{
	VkImageSubresourceRange imageSubresourceRange = {};
	imageSubresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
	imageSubresourceRange.baseArrayLayer = 0;
	imageSubresourceRange.baseMipLevel = 0;
	imageSubresourceRange.layerCount = 1;
	imageSubresourceRange.levelCount = 1;

	return imageSubresourceRange;
}

static VkImageView createImageView(const VkDevice& logicalDevice, const VkImage& image, const VkFormat& format)
{
	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.components = createComponentMapping();
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.format = format;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.subresourceRange = createImageSubresourceRange();
	imageViewCreateInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;

	VkImageView imageView;
	handleError(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &imageView), "Failed to create image view.");

	return imageView;
}

static LogicalDeviceInfo createLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	const float queuePriority = 1.0f;
	QueueFamilyIndexInfo queueInfo = getVulkanQueueInfo(physicalDevice, surface);
	VkDeviceQueueCreateInfo graphicsQueueCreateInfo = createDeviceQueueCreateInfo(queueInfo.graphicsQueueFamilyIndex, queuePriority);
	VkDeviceQueueCreateInfo presentQueueCreateInfo = createDeviceQueueCreateInfo(queueInfo.presentQueueFamilyIndex, queuePriority);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = getDeviceCreateInfos(graphicsQueueCreateInfo, presentQueueCreateInfo);

	VkDeviceCreateInfo info = {};
	info.enabledExtensionCount = DEVICE_EXTENSIONS.size();
	info.enabledLayerCount = 0;
	info.flags = 0;
	info.pEnabledFeatures = nullptr;
	info.pNext = nullptr;
	info.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();
	info.ppEnabledLayerNames = nullptr;
	info.pQueueCreateInfos = queueCreateInfos.data();
	info.queueCreateInfoCount = queueCreateInfos.size();
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	VkDevice device;
	handleError(vkCreateDevice(physicalDevice, &info, nullptr, &device), "Failed to create logical device.");

	VkQueue graphicsQueue;
	vkGetDeviceQueue(device, graphicsQueueCreateInfo.queueFamilyIndex, 0, &graphicsQueue);

	VkQueue presentQueue;
	vkGetDeviceQueue(device, presentQueueCreateInfo.queueFamilyIndex, 0, &presentQueue);

	QueueInfo graphicsInfo = { queueInfo.graphicsQueueFamilyIndex, graphicsQueue };
	QueueInfo presentInfo = { queueInfo.presentQueueFamilyIndex, presentQueue };

	return { device, graphicsInfo, presentInfo };
}

static VkPhysicalDevice createPhysicalDevice(const VkInstance& instance)
{
	uint32_t physicalDeviceCount;
	handleError(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr), "Failed to get physical device.");

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	handleError(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()), "Failed to get physical device.");

	return physicalDevices[0];
}

static VkPipelineRasterizationStateCreateInfo createPipelineRasterizationStateCreateInfo()
{
	VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo = {};
	rasterizationCreateInfo.cullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
	rasterizationCreateInfo.depthBiasClamp = 0.0f;
	rasterizationCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationCreateInfo.depthBiasSlopeFactor = 0.0f;
	rasterizationCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationCreateInfo.flags = 0;
	rasterizationCreateInfo.frontFace = VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
	rasterizationCreateInfo.lineWidth = 1.0f;
	rasterizationCreateInfo.pNext = nullptr;
	rasterizationCreateInfo.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
	rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

	return rasterizationCreateInfo;
}

static VkPipelineInputAssemblyStateCreateInfo createPipelineInputAssemblyStateCreateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
	pipelineInputAssemblyStateCreateInfo.flags = 0;
	pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
	pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
	pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pipelineInputAssemblyStateCreateInfo.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	return pipelineInputAssemblyStateCreateInfo;
}

static VkRenderPass createRenderPass(const VkDevice& logicalDevice, const VkFormat& format)
{
	VkAttachmentDescription attachmentDescription = createAttachmentDescription(format);
	VkAttachmentReference attachmentReference = createAttachmentReference();
	VkSubpassDescription subpassDescription = createSubpassDescription(attachmentReference);

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.dependencyCount = 0;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.pAttachments = &attachmentDescription;
	renderPassCreateInfo.pDependencies = nullptr;
	renderPassCreateInfo.pNext = nullptr;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.subpassCount = 1;

	VkRenderPass renderPass;
	handleError(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &renderPass), "Failed to create render pass.");

	return renderPass;
}

static VkSemaphore createSemaphore(const VkDevice& logicalDevice)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.flags = 0;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore semaphore;
	vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &semaphore);

	return semaphore;
}

static VkShaderModule createShaderModule(const VkDevice& logicalDevice, const std::string& spirvPath)
{
	std::vector<char> shaderCode = getShaderCode(spirvPath);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.codeSize = shaderCode.size();
	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
	shaderModuleCreateInfo.pNext = nullptr;
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	VkShaderModule shaderModule;
	handleError(vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule), "Failed to create shader module from " + spirvPath + " .");

	return shaderModule;
}

static VkPipelineShaderStageCreateInfo createShaderStageCreateInfo(const VkDevice& logicalDevice, const VkShaderStageFlagBits& shaderType, const std::string& spirvPath)
{
	VkShaderModule shaderModule = createShaderModule(logicalDevice, spirvPath);

	VkPipelineShaderStageCreateInfo shaderCreateInfo = {};
	shaderCreateInfo.flags = 0;
	shaderCreateInfo.module = shaderModule;
	shaderCreateInfo.pName = "main";
	shaderCreateInfo.pNext = nullptr;
	shaderCreateInfo.pSpecializationInfo = nullptr;
	shaderCreateInfo.stage = shaderType;
	shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

	return shaderCreateInfo;
}

static VkSubpassDescription createSubpassDescription(const VkAttachmentReference& attachmentReference)
{
	VkSubpassDescription subpassDescription = {};
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.flags = 0;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pColorAttachments = &attachmentReference;
	subpassDescription.pDepthStencilAttachment = nullptr;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.pPreserveAttachments = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pResolveAttachments = nullptr;

	return subpassDescription;
}

static VkSurfaceKHR createSurface(const VkInstance& vkInstance, GLFWwindow* glfwWindow)
{
	VkSurfaceKHR vkSurface;
	handleError(glfwCreateWindowSurface(vkInstance, glfwWindow, nullptr, &vkSurface), "Failed to create surface.");

	return vkSurface;
}

static VkSwapchainKHR createSwapchain(const VkDevice& logicalDevice, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const VkSurfaceFormatKHR& surfaceFormat, const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	uint32_t presentModeCount;
	handleError(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr), "Failed to get surface present modes.");
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	handleError(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()), "Failed to get surface present modes.");

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.clipped = VK_FALSE;
	swapchainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	swapchainCreateInfo.pNext = nullptr;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.presentMode = presentModes[0];
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;

	VkSwapchainKHR swapchain;
	handleError(vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &swapchain), "Failed to create swapchain");

	return swapchain;
}

static VkPipelineVertexInputStateCreateInfo createVertexInputStateCreateInfo()
{
	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {};
	pipelineVertexInputStateCreateInfo.flags = 0;
	pipelineVertexInputStateCreateInfo.pNext = nullptr;
	pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;
	pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr;
	pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
	pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;

	return pipelineVertexInputStateCreateInfo;
}

static VkBool32 debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	std::cerr << pMessage << "\n\n";
	return VK_FALSE;
}

static std::vector<VkDeviceQueueCreateInfo> getDeviceCreateInfos(const VkDeviceQueueCreateInfo& graphicsQueueCreateInfo, const VkDeviceQueueCreateInfo& presentQueueCreateInfo)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	if (graphicsQueueCreateInfo.queueFamilyIndex == presentQueueCreateInfo.queueFamilyIndex)
	{
		queueCreateInfos = { graphicsQueueCreateInfo };
	}
	else
	{
		queueCreateInfos = { graphicsQueueCreateInfo, presentQueueCreateInfo };
	}

	return queueCreateInfos;
}

static std::vector<char> getShaderCode(const std::string& spirvPath)
{
	std::ifstream spirvFileStream(spirvPath, std::ios::binary);

	std::vector<char> spirvCode;
	if (spirvFileStream.is_open())
	{
		spirvCode = std::vector<char>(std::istreambuf_iterator<char>(spirvFileStream), std::istreambuf_iterator<char>());
	}

	return spirvCode;
}

static std::vector<VkImage> getSwapchainImages(const VkDevice& logicalDevice, const VkSwapchainKHR& swapchain)
{
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr);

	std::vector<VkImage> swapchainImages(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, swapchainImages.data());

	return swapchainImages;
}

static std::vector<const char*> getVulkanInstanceExtensions()
{
	uint32_t extensionCount;
	const char** ppExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	std::vector<const char*> extensions(extensionCount);
	std::copy(ppExtensions, ppExtensions + extensionCount, extensions.begin());

	return extensions;
}

static QueueFamilyIndexInfo getVulkanQueueInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	uint32_t queueFamilyPropertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

	QueueFamilyIndexInfo queueInfo = {};

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

static void recordCommandBuffer(const VkCommandBuffer& commandBuffer, const VkExtent2D& extent, const VkFramebuffer& framebuffer, const VkPipeline& graphicsPipeline, const VkRenderPass& renderPass)
{
	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.flags = 0;
	commandBufferBeginInfo.pInheritanceInfo = nullptr;
	commandBufferBeginInfo.pNext = nullptr;
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VkClearValue clearValue;
	clearValue.color = { 0.0f, 0.0f, 0.0f, 1.0f };

	VkRect2D renderArea;
	renderArea.extent = extent;
	renderArea.offset = { 0, 0 };

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.framebuffer = framebuffer;
	renderPassBeginInfo.pClearValues = &clearValue;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderArea = renderArea;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

	handleError(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo), "Failed to begin recording command buffer.");

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	handleError(vkEndCommandBuffer(commandBuffer), "Failed to finish recording command buffer.");
}

static void vulkanDestroyDebugReportCallbackEXT(const VkInstance& instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
	vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

static VkResult vulkanCreateDebugReportCallbackEXT(const VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
	return vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}