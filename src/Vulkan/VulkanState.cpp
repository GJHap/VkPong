#include "VulkanState.hpp"
#include "VulkanExtensions.hpp"
#include "VulkanObjectCreation/VulkanObjectCreation.hpp"

namespace vkPong
{
	static vk::SurfaceFormatKHR chooseSurfaceFormat(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
	{
		return physicalDevice.getSurfaceFormatsKHR(surface)[0];
	}

	VulkanState::VulkanState(GLFWwindow* glfwWindow)
		: m_instance(createInstance())
	{
		m_debugReportCallback = createDebugReportCallback(m_instance);
		m_physicalDevice = createPhysicalDevice(m_instance);
		m_surface = createSurface(m_instance, glfwWindow);

		LogicalDeviceInfo logicalDeviceInfo = createLogicalDevice(m_physicalDevice, m_surface);
		m_logicalDevice = logicalDeviceInfo.logicalDevice;
		m_graphicsQueueInfo = logicalDeviceInfo.graphicsQueueInfo;
		m_presentQueueInfo = logicalDeviceInfo.presentQueueInfo;
		m_commandPool = createCommandPool(m_logicalDevice, m_graphicsQueueInfo.queueFamilyIndex);
		m_renderPass = createRenderPass(m_logicalDevice, chooseSurfaceFormat(m_physicalDevice, m_surface).format);
		m_graphicsPipelineLayout = createGraphicsPipelineLayout(m_logicalDevice);
		setupSwapchain();
	}

	VulkanState::~VulkanState()
	{
		cleanupSwapchain();
		m_logicalDevice.destroyPipelineLayout(m_graphicsPipelineLayout);
		m_logicalDevice.destroyRenderPass(m_renderPass);
		m_logicalDevice.destroyCommandPool(m_commandPool);
		m_logicalDevice.destroy();
		m_instance.destroySurfaceKHR(m_surface);
#ifndef NDEBUG
		m_instance.destroyDebugReportCallbackEXT(m_debugReportCallback, nullptr, getVkDestroyDebugReportCallbackEXTDispatchLoader(m_instance));
#endif
		m_instance.destroy();
	}

	BufferInfo& VulkanState::ballVertexBuffer(const uint32_t& index)
	{
		return m_ballVertexBuffers[index];
	}

	const vk::CommandBuffer& VulkanState::commandBuffer(const uint32_t& index) const
	{
		return m_commandBuffers[index];
	}

	void VulkanState::cleanupSwapchain()
	{
		m_logicalDevice.waitIdle();
		for (uint32_t i = 0; i < swapchainImageCount(); ++i)
		{
			m_logicalDevice.destroyFence(m_fences[i]);
			m_logicalDevice.destroySemaphore(m_imageRenderedSemaphores[i]);
			m_logicalDevice.destroySemaphore(m_imageAvailableSemaphores[i]);
			m_ballVertexBuffers[i].free(m_logicalDevice);
			m_ballVertexBuffers[i].setNullHandles();
			m_opponentVertexBuffers[i].free(m_logicalDevice);
			m_opponentVertexBuffers[i].setNullHandles();
			m_playerVertexBuffers[i].free(m_logicalDevice);
			m_playerVertexBuffers[i].setNullHandles();
			m_logicalDevice.destroyFramebuffer(m_swapchainFramebuffers[i]);
			m_logicalDevice.destroyImageView(m_swapchainImageViews[i]);
		}
		m_logicalDevice.freeCommandBuffers(m_commandPool, vk::ArrayProxy<const vk::CommandBuffer>{m_commandBuffers});
		m_logicalDevice.destroySwapchainKHR(m_swapchain);
		m_logicalDevice.destroyPipeline(m_graphicsPipeline);
	}

	const vk::Fence& VulkanState::fence(const uint32_t& index) const
	{
		return m_fences[index];
	}

	const vk::Framebuffer& VulkanState::framebuffer(const uint32_t& index) const
	{
		return m_swapchainFramebuffers[index];
	}

	const vk::Pipeline& VulkanState::graphicsPipeline() const
	{
		return m_graphicsPipeline;
	}

	const vk::Queue& VulkanState::graphicsQueue() const
	{
		return m_graphicsQueueInfo.queue;
	}

	const vk::Semaphore& VulkanState::imageAvailableSemaphore(const uint32_t& index) const
	{
		return m_imageAvailableSemaphores[index];
	}

	const vk::Semaphore& VulkanState::imageRenderedSemaphore(const uint32_t& index) const
	{
		return m_imageRenderedSemaphores[index];
	}

	const vk::Device& VulkanState::logicalDevice() const
	{
		return m_logicalDevice;
	}

	BufferInfo& VulkanState::opponentVertexBuffer(const uint32_t& index)
	{
		return m_opponentVertexBuffers[index];
	}

	const vk::PhysicalDevice& VulkanState::physicalDevice() const
	{
		return m_physicalDevice;
	}

	BufferInfo& VulkanState::playerVertexBuffer(const uint32_t& index)
	{
		return m_playerVertexBuffers[index];
	}

	const vk::Queue& VulkanState::presentQueue() const
	{
		return m_presentQueueInfo.queue;
	}

	void VulkanState::setupSwapchain()
	{
		vk::SurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(m_physicalDevice, m_surface);
		vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);
		m_surfaceExtent = surfaceCapabilities.currentExtent;

		m_graphicsPipeline = createGraphicsPipeline(m_graphicsPipelineLayout, m_logicalDevice, m_renderPass, m_surfaceExtent);
		m_swapchain = createSwapchain(m_logicalDevice, m_physicalDevice, m_surface, surfaceFormat, surfaceCapabilities);
		m_swapchainImages = m_logicalDevice.getSwapchainImagesKHR(m_swapchain);
		m_swapchainImageViews.resize(swapchainImageCount());
		std::transform(m_swapchainImages.cbegin(), m_swapchainImages.cend(), m_swapchainImageViews.begin(), [this, surfaceFormat](VkImage swapchainImage)
		{
			return createImageView(m_logicalDevice, swapchainImage, surfaceFormat.format);
		});
		m_swapchainFramebuffers.resize(swapchainImageCount());
		std::transform(m_swapchainImageViews.cbegin(), m_swapchainImageViews.cend(), m_swapchainFramebuffers.begin(), [this](VkImageView swapchainImageView)
		{
			return createFramebuffer(m_logicalDevice, swapchainImageView, m_renderPass, m_surfaceExtent);
		});

		m_playerVertexBuffers.resize(swapchainImageCount());
		m_opponentVertexBuffers.resize(swapchainImageCount());
		m_ballVertexBuffers.resize(swapchainImageCount());
		m_commandBuffers.resize(swapchainImageCount());
		std::generate_n(m_commandBuffers.begin(), swapchainImageCount(), [this]() { return createCommandBuffer(m_commandPool, m_logicalDevice); });
		m_imageAvailableSemaphores.resize(swapchainImageCount());
		std::generate_n(m_imageAvailableSemaphores.begin(), swapchainImageCount(), [this]() { return createSemaphore(m_logicalDevice); });
		m_imageRenderedSemaphores.resize(swapchainImageCount());
		std::generate_n(m_imageRenderedSemaphores.begin(), swapchainImageCount(), [this]() { return createSemaphore(m_logicalDevice); });
		m_fences.resize(swapchainImageCount());
		std::generate_n(m_fences.begin(), swapchainImageCount(), [this]() { return createFence(m_logicalDevice); });
	}

	const vk::RenderPass& VulkanState::renderPass() const
	{
		return m_renderPass;
	}

	void VulkanState::resetSwapchain()
	{
		cleanupSwapchain();
		setupSwapchain();
	}

	const vk::Extent2D& VulkanState::surfaceExtent() const
	{
		return m_surfaceExtent;
	}

	const vk::SwapchainKHR& VulkanState::swapchain() const
	{
		return m_swapchain;
	}

	uint32_t VulkanState::swapchainImageCount() const
	{
		return static_cast<uint32_t>(m_swapchainImages.size());
	}
}