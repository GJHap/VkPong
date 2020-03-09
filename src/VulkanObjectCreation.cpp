#include "VulkanObjectCreation.hpp"
#include "VulkanExtensions.hpp"

#include <fstream>
#include <iostream>

namespace vkPong
{
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

	struct QueueFamilyIndexInfo
	{
		uint32_t graphicsQueueFamilyIndex;
		uint32_t presentQueueFamilyIndex;
	};

	static std::vector<const char*> getVulkanInstanceExtensions()
	{
		uint32_t extensionCount;
		const char** ppExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

		std::vector<const char*> extensions(extensionCount);
		std::copy(ppExtensions, ppExtensions + extensionCount, extensions.begin());

		return extensions;
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

	static VkShaderModule createShaderModule(const vk::Device& logicalDevice, const std::string& spirvPath)
	{
		std::vector<char> shaderCode = getShaderCode(spirvPath);
		vk::ShaderModuleCreateInfo shaderModuleCreateInfo;
		shaderModuleCreateInfo.setCodeSize(shaderCode.size());
		shaderModuleCreateInfo.setPCode(reinterpret_cast<const uint32_t*>(shaderCode.data()));

		return logicalDevice.createShaderModule(shaderModuleCreateInfo);
	}

	static vk::PipelineShaderStageCreateInfo createPipelineShaderStageCreateInfo(const vk::Device logicalDevice, const vk::ShaderStageFlagBits& stage, const std::string& spirvPath)
	{
		vk::PipelineShaderStageCreateInfo shaderStageCreateInfo;
		shaderStageCreateInfo.setStage(stage);
		shaderStageCreateInfo.setModule(createShaderModule(logicalDevice, spirvPath));
		shaderStageCreateInfo.setPName("main");

		return shaderStageCreateInfo;
	}

	static std::vector<vk::DeviceQueueCreateInfo> getDeviceQueueCreateInfos(const vk::DeviceQueueCreateInfo& graphicsQueueCreateInfo, const vk::DeviceQueueCreateInfo& presentQueueCreateInfo)
	{
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

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

	static QueueFamilyIndexInfo getVulkanQueueInfo(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
	{
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

		QueueFamilyIndexInfo queueInfo = {};
		for (std::vector<vk::QueueFamilyProperties>::size_type i = 0; i < queueFamilyProperties.size(); ++i)
		{
			if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics)
			{
				queueInfo.graphicsQueueFamilyIndex = i;
			}

			if (physicalDevice.getSurfaceSupportKHR(i, surface))
			{
				queueInfo.presentQueueFamilyIndex = i;
			}
		}

		return queueInfo;
	}

	static void recordCommandBuffer(const vk::CommandBuffer& commandBuffer, const vk::Extent2D& extent, const vk::Framebuffer& framebuffer, const vk::Pipeline& graphicsPipeline, const vk::RenderPass& renderPass)
	{
		vk::CommandBufferBeginInfo commandBufferBeginInfo;

		vk::ClearValue clearValue;
		clearValue.setColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

		vk::Rect2D renderArea;
		renderArea.setExtent(extent);

		vk::RenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.setClearValueCount(1);
		renderPassBeginInfo.setPClearValues(&clearValue);
		renderPassBeginInfo.setFramebuffer(framebuffer);
		renderPassBeginInfo.setRenderArea(renderArea);
		renderPassBeginInfo.setRenderPass(renderPass);

		commandBuffer.begin(commandBufferBeginInfo);
		commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
		commandBuffer.draw(3, 1, 0, 0);
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	static VkBool32 debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
	{
		std::cerr << pMessage << "\n\n";
		return VK_FALSE;
	}

	static vk::DeviceQueueCreateInfo createDeviceQueueCreateInfo(const float& queuePriority, const uint32_t& queueFamilyIndex)
	{
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo;
		deviceQueueCreateInfo.setPQueuePriorities(&queuePriority);
		deviceQueueCreateInfo.setQueueFamilyIndex(queueFamilyIndex);
		deviceQueueCreateInfo.setQueueCount(1);

		return deviceQueueCreateInfo;
	}

	vk::CommandBuffer createCommandBuffer(const vk::CommandPool& commandPool, const vk::Device& logicalDevice, const vk::Extent2D& extent, const vk::Framebuffer& framebuffer, const vk::Pipeline& graphicsPipeline, const vk::RenderPass& renderPass)
	{
		vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
		commandBufferAllocateInfo.setCommandBufferCount(1);
		commandBufferAllocateInfo.setCommandPool(commandPool);

		vk::CommandBuffer commandBuffer = logicalDevice.allocateCommandBuffers(commandBufferAllocateInfo)[0];

		recordCommandBuffer(commandBuffer, extent, framebuffer, graphicsPipeline, renderPass);

		return commandBuffer;
	}

	vk::CommandPool createCommandPool(const vk::Device& logicalDevice, const uint32_t& queueFamilyIndex)
	{
		vk::CommandPoolCreateInfo commandPoolCreateInfo;
		commandPoolCreateInfo.setQueueFamilyIndex(queueFamilyIndex);

		return logicalDevice.createCommandPool(commandPoolCreateInfo);
	}

	vk::DebugReportCallbackEXT createDebugReportCallback(const vk::Instance& instance)
	{
		vk::DebugReportCallbackEXT debugReportCallback;

		if (enableValidation)
		{
			vk::DebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;
			debugReportCallbackCreateInfo.setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eDebug);
			debugReportCallbackCreateInfo.setPfnCallback(&debugCallback);

			debugReportCallback = instance.createDebugReportCallbackEXT(debugReportCallbackCreateInfo, nullptr, getVkCreateDebugReportCallbackEXTDispatchLoader(instance));
		}

		return debugReportCallback;
	}

	vk::Fence createFence(const vk::Device& logicalDevice)
	{
		vk::FenceCreateInfo fenceCreateInfo;
		fenceCreateInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

		return logicalDevice.createFence(fenceCreateInfo);
	}

	vk::Framebuffer createFramebuffer(const vk::Device& logicalDevice, const vk::ImageView& imageView, const vk::RenderPass& renderPass, const vk::Extent2D& extent)
	{
		vk::FramebufferCreateInfo framebufferCreateInfo;
		framebufferCreateInfo.setAttachmentCount(1);
		framebufferCreateInfo.setPAttachments(&imageView);
		framebufferCreateInfo.setHeight(extent.height);
		framebufferCreateInfo.setWidth(extent.width);
		framebufferCreateInfo.setRenderPass(renderPass);
		framebufferCreateInfo.setLayers(1);

		return logicalDevice.createFramebuffer(framebufferCreateInfo);
	}

	vk::Pipeline createGraphicsPipeline(const vk::PipelineLayout& graphicsLayout, const vk::Device& logicalDevice, const vk::RenderPass& renderPass, const vk::Extent2D& extent)
	{
		vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;
		pipelineInputAssemblyStateCreateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);

		vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
		pipelineRasterizationStateCreateInfo.setFrontFace(vk::FrontFace::eClockwise);
		pipelineRasterizationStateCreateInfo.setPolygonMode(vk::PolygonMode::eFill);
		pipelineRasterizationStateCreateInfo.setLineWidth(1.0f);

		vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;

		std::vector<vk::PipelineShaderStageCreateInfo> shaderCreateInfos
		{
			createPipelineShaderStageCreateInfo(logicalDevice, vk::ShaderStageFlagBits::eVertex, "./shaders/vertex/shader.spv"),
			createPipelineShaderStageCreateInfo(logicalDevice, vk::ShaderStageFlagBits::eFragment, "./shaders/fragment/shader.spv"),
		};

		vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

		vk::Rect2D scissor;
		scissor.setExtent(extent);

		vk::Viewport viewport;
		viewport.setWidth(extent.width);
		viewport.setHeight(extent.height);
		viewport.setMaxDepth(1.0f);

		vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
		pipelineViewportStateCreateInfo.setViewportCount(1);
		pipelineViewportStateCreateInfo.setPViewports(&viewport);
		pipelineViewportStateCreateInfo.setScissorCount(1);
		pipelineViewportStateCreateInfo.setPScissors(&scissor);

		vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
		pipelineColorBlendAttachmentState.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
		pipelineColorBlendAttachmentState.setBlendEnable(1);
		pipelineColorBlendAttachmentState.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha);
		pipelineColorBlendAttachmentState.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha);
		pipelineColorBlendAttachmentState.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);

		vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;
		pipelineColorBlendStateCreateInfo.setAttachmentCount(1);
		pipelineColorBlendStateCreateInfo.setPAttachments(&pipelineColorBlendAttachmentState);

		vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
		graphicsPipelineCreateInfo.setLayout(graphicsLayout);
		graphicsPipelineCreateInfo.setPColorBlendState(&pipelineColorBlendStateCreateInfo);
		graphicsPipelineCreateInfo.setPInputAssemblyState(&pipelineInputAssemblyStateCreateInfo);
		graphicsPipelineCreateInfo.setPMultisampleState(&pipelineMultisampleStateCreateInfo);
		graphicsPipelineCreateInfo.setPRasterizationState(&pipelineRasterizationStateCreateInfo);
		graphicsPipelineCreateInfo.setPStages(shaderCreateInfos.data());
		graphicsPipelineCreateInfo.setStageCount(shaderCreateInfos.size());
		graphicsPipelineCreateInfo.setPVertexInputState(&pipelineVertexInputStateCreateInfo);
		graphicsPipelineCreateInfo.setPViewportState(&pipelineViewportStateCreateInfo);
		graphicsPipelineCreateInfo.setRenderPass(renderPass);

		vk::Pipeline graphicsPipeline = logicalDevice.createGraphicsPipeline(vk::PipelineCache(), graphicsPipelineCreateInfo);

		for (VkPipelineShaderStageCreateInfo shaderCreateInfo : shaderCreateInfos)
		{
			logicalDevice.destroyShaderModule(shaderCreateInfo.module);
		}

		return graphicsPipeline;
	}

	vk::PipelineLayout createGraphicsPipelineLayout(const vk::Device& logicalDevice)
	{
		return logicalDevice.createPipelineLayout(vk::PipelineLayoutCreateInfo());
	}

	vk::ImageView createImageView(const vk::Device& logicalDevice, const vk::Image& image, const vk::Format& format)
	{
		vk::ImageSubresourceRange imageSubresourceRange;
		imageSubresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor);
		imageSubresourceRange.setLayerCount(1);
		imageSubresourceRange.setLevelCount(1);

		vk::ImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.setFormat(format);
		imageViewCreateInfo.setImage(image);
		imageViewCreateInfo.setViewType(vk::ImageViewType::e2D);
		imageViewCreateInfo.setSubresourceRange(imageSubresourceRange);

		return logicalDevice.createImageView(imageViewCreateInfo);
	}

	vk::Instance createInstance()
	{
		std::vector<const char*> instanceExtensions = getVulkanInstanceExtensions();
		std::vector<const char*> enabledLayers;

		if (enableValidation)
		{
			enabledLayers = INSTANCE_VALIDATION_LAYERS;
			std::copy(INSTANCE_EXTENSIONS.begin(), INSTANCE_EXTENSIONS.end(), std::back_inserter(instanceExtensions));
		}

		vk::ApplicationInfo applicationInfo;
		applicationInfo.setPApplicationName("VkPong");
		applicationInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
		applicationInfo.setApiVersion(VK_API_VERSION_1_1);

		vk::InstanceCreateInfo instanceCreateInfo;
		instanceCreateInfo.setPApplicationInfo(&applicationInfo);
		instanceCreateInfo.setEnabledLayerCount(enabledLayers.size());
		instanceCreateInfo.setPpEnabledLayerNames(enabledLayers.data());
		instanceCreateInfo.setEnabledExtensionCount(instanceExtensions.size());
		instanceCreateInfo.setPpEnabledExtensionNames(instanceExtensions.data());

		return vk::createInstance(instanceCreateInfo);
	}

	LogicalDeviceInfo createLogicalDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
	{
		const float queuePriority = 1.0f;
		QueueFamilyIndexInfo queueFamilyIndexInfo = getVulkanQueueInfo(physicalDevice, surface);
		std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos = getDeviceQueueCreateInfos(createDeviceQueueCreateInfo(queuePriority, queueFamilyIndexInfo.graphicsQueueFamilyIndex), createDeviceQueueCreateInfo(queuePriority, queueFamilyIndexInfo.presentQueueFamilyIndex));

		vk::DeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.setQueueCreateInfoCount(1);
		deviceCreateInfo.setPQueueCreateInfos(deviceQueueCreateInfos.data());
		deviceCreateInfo.setEnabledExtensionCount(DEVICE_EXTENSIONS.size());
		deviceCreateInfo.setPpEnabledExtensionNames(DEVICE_EXTENSIONS.data());

		vk::Device logicalDevice = physicalDevice.createDevice(deviceCreateInfo);
		vk::Queue graphicsQueue = logicalDevice.getQueue(queueFamilyIndexInfo.graphicsQueueFamilyIndex, 0);
		vk::Queue presentQueue = logicalDevice.getQueue(queueFamilyIndexInfo.presentQueueFamilyIndex, 0);

		QueueInfo graphicsInfo = { queueFamilyIndexInfo.graphicsQueueFamilyIndex, graphicsQueue };
		QueueInfo presentInfo = { queueFamilyIndexInfo.presentQueueFamilyIndex, presentQueue };

		return { logicalDevice, graphicsInfo, presentInfo };
	}

	vk::PhysicalDevice createPhysicalDevice(const vk::Instance& instance)
	{
		return instance.enumeratePhysicalDevices()[0];
	}

	vk::RenderPass createRenderPass(const vk::Device& logicalDevice, const vk::Format& format)
	{
		vk::AttachmentDescription attachmentDescription;
		attachmentDescription.setFormat(format);
		attachmentDescription.setLoadOp(vk::AttachmentLoadOp::eDontCare);
		attachmentDescription.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
		attachmentDescription.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
		attachmentDescription.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		vk::AttachmentReference attachmentReference;
		attachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		vk::SubpassDescription subpassDescription;
		subpassDescription.setColorAttachmentCount(1);
		subpassDescription.setPColorAttachments(&attachmentReference);

		vk::RenderPassCreateInfo renderPassCreateInfo;
		renderPassCreateInfo.setAttachmentCount(1);
		renderPassCreateInfo.setPAttachments(&attachmentDescription);
		renderPassCreateInfo.setSubpassCount(1);
		renderPassCreateInfo.setPSubpasses(&subpassDescription);

		return logicalDevice.createRenderPass(renderPassCreateInfo);
	}

	vk::Semaphore createSemaphore(const vk::Device& logicalDevice)
	{
		return logicalDevice.createSemaphore(vk::SemaphoreCreateInfo());
	}

	vk::SurfaceKHR createSurface(const vk::Instance& instance, GLFWwindow* glfwWindow)
	{
		VkSurfaceKHR surface;
		glfwCreateWindowSurface(instance.operator VkInstance(), glfwWindow, nullptr, &surface);

		return vk::SurfaceKHR(surface);
	}

	vk::SwapchainKHR createSwapchain(const vk::Device& logicalDevice, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::SurfaceFormatKHR& surfaceFormat, const vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

		vk::SwapchainCreateInfoKHR swapchainCreateInfo;
		swapchainCreateInfo.setImageArrayLayers(1);
		swapchainCreateInfo.setImageColorSpace(surfaceFormat.colorSpace);
		swapchainCreateInfo.setImageExtent(surfaceCapabilities.currentExtent);
		swapchainCreateInfo.setImageFormat(surfaceFormat.format);
		swapchainCreateInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
		swapchainCreateInfo.setMinImageCount(surfaceCapabilities.minImageCount);
		swapchainCreateInfo.setPresentMode(presentModes[0]);
		swapchainCreateInfo.setPreTransform(surfaceCapabilities.currentTransform);
		swapchainCreateInfo.setSurface(surface);

		return logicalDevice.createSwapchainKHR(swapchainCreateInfo);
	}
}