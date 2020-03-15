#include "VulkanState.hpp"
#include "VulkanExtensions.hpp"
#include "VulkanObjectCreation/VulkanObjectCreation.hpp"

namespace vkPong
{
	VulkanState::VulkanState(GLFWwindow* glfwWindow)
		: m_instance(createInstance())
	{
		m_debugReportCallback = createDebugReportCallback(m_instance);
		m_physicalDevice = createPhysicalDevice(m_instance);
		m_surface = createSurface(m_instance, glfwWindow);

		LogicalDeviceInfo logicalDeviceInfo = createLogicalDevice(m_physicalDevice, m_surface);
		m_logicalDevice = logicalDeviceInfo.logicalDevice;
		m_graphicsQueue = logicalDeviceInfo.graphicsQueueInfo.queue;
		m_presentQueue = logicalDeviceInfo.presentQueueInfo.queue;

		m_bufferInfo = createBuffer(m_logicalDevice, m_physicalDevice);

		std::vector<vk::SurfaceFormatKHR> surfaceFormats = m_physicalDevice.getSurfaceFormatsKHR(m_surface);
		vk::SurfaceFormatKHR surfaceFormat = surfaceFormats[0];
		vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

		m_renderPass = createRenderPass(m_logicalDevice, surfaceFormat.format);
		m_graphicsPipelineLayout = createGraphicsPipelineLayout(m_logicalDevice);
		m_graphicsPipeline = createGraphicsPipeline(m_graphicsPipelineLayout, m_logicalDevice, m_renderPass, surfaceCapabilities.currentExtent);
		m_swapchain = createSwapchain(m_logicalDevice, m_physicalDevice, m_surface, surfaceFormat, surfaceCapabilities);
		m_swapchainImages = m_logicalDevice.getSwapchainImagesKHR(m_swapchain);
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
			return createCommandBuffer(m_commandPool, m_logicalDevice, surfaceCapabilities.currentExtent, framebuffer, m_graphicsPipeline, m_renderPass, m_bufferInfo.buffer);
		});
		std::generate_n(std::back_inserter(m_imageAvailableSemaphores), m_swapchainFramebuffers.size(), [this]() { return createSemaphore(m_logicalDevice); });
		std::generate_n(std::back_inserter(m_imageRenderedSemaphores), m_swapchainFramebuffers.size(), [this]() { return createSemaphore(m_logicalDevice); });
		std::generate_n(std::back_inserter(m_fences), m_swapchainFramebuffers.size(), [this]() { return createFence(m_logicalDevice); });
	}

	VulkanState::~VulkanState()
	{
		m_logicalDevice.waitIdle();
		for (vk::Fence& fence : m_fences) m_logicalDevice.destroyFence(fence);
		for (vk::Semaphore& semaphore : m_imageRenderedSemaphores) m_logicalDevice.destroySemaphore(semaphore);
		for (vk::Semaphore& semaphore : m_imageAvailableSemaphores) m_logicalDevice.destroySemaphore(semaphore);
		m_logicalDevice.freeCommandBuffers(m_commandPool, vk::ArrayProxy<const vk::CommandBuffer>(m_commandBuffers));
		m_logicalDevice.destroyCommandPool(m_commandPool);
		for (vk::Framebuffer& framebuffer : m_swapchainFramebuffers) m_logicalDevice.destroyFramebuffer(framebuffer);
		for (vk::ImageView& imageView : m_swapchainImageViews) m_logicalDevice.destroyImageView(imageView);
		m_logicalDevice.destroySwapchainKHR(m_swapchain);
		m_logicalDevice.destroyPipeline(m_graphicsPipeline);
		m_logicalDevice.destroyPipelineLayout(m_graphicsPipelineLayout);
		m_logicalDevice.destroyRenderPass(m_renderPass);
		m_logicalDevice.freeMemory(m_bufferInfo.bufferMemory);
		m_logicalDevice.destroyBuffer(m_bufferInfo.buffer);
		m_logicalDevice.destroy();
		m_instance.destroySurfaceKHR(m_surface);
		m_instance.destroyDebugReportCallbackEXT(m_debugReportCallback, nullptr, getVkDestroyDebugReportCallbackEXTDispatchLoader(m_instance));
		m_instance.destroy();
	}

	vk::CommandBuffer VulkanState::commandBuffer(const uint32_t& index) const
	{
		return m_commandBuffers[index];
	}

	vk::Fence VulkanState::fence(const uint32_t& index) const
	{
		return m_fences[index];
	}

	vk::Queue VulkanState::graphicsQueue() const
	{
		return m_graphicsQueue;
	}

	vk::Semaphore VulkanState::imageAvailableSemaphore(const uint32_t& index) const
	{
		return m_imageAvailableSemaphores[index];
	}

	vk::Semaphore VulkanState::imageRenderedSemaphore(const uint32_t& index) const
	{
		return m_imageRenderedSemaphores[index];
	}

	vk::Device VulkanState::logicalDevice() const
	{
		return m_logicalDevice;
	}

	vk::Queue VulkanState::presentQueue() const
	{
		return m_presentQueue;
	}

	vk::SwapchainKHR VulkanState::swapchain() const
	{
		return m_swapchain;
	}

	uint32_t VulkanState::swapchainImageCount() const
	{
		return static_cast<uint32_t>(m_swapchainImages.size());
	}
}