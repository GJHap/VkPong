#pragma once

#include "vulkanhpp_include.hpp"
#include "Structs/BufferInfo.hpp"
#include "Structs\VertexData.hpp"

#include <GLFW\glfw3.h>

namespace vkPong
{
	class VulkanState
	{
	public:
		std::vector<BufferInfo> m_ballVertexBuffers;
		std::vector<vk::CommandBuffer> m_commandBuffers;
		vk::CommandPool m_commandPool;
		vk::DebugReportCallbackEXT m_debugReportCallback;
		std::vector<vk::Fence> m_fences;
		vk::Pipeline m_graphicsPipeline;
		vk::PipelineLayout m_graphicsPipelineLayout;
		vk::Queue m_graphicsQueue;
		std::vector<vk::Semaphore> m_imageAvailableSemaphores;
		std::vector<vk::Semaphore> m_imageRenderedSemaphores;
		vk::Instance m_instance;
		vk::Device m_logicalDevice;
		std::vector<BufferInfo> m_opponentVertexBuffers;
		vk::PhysicalDevice m_physicalDevice;
		std::vector<BufferInfo> m_playerVertexBuffers;
		vk::Queue m_presentQueue;
		vk::RenderPass m_renderPass;
		vk::SurfaceKHR m_surface;
		vk::Extent2D m_surfaceExtent;
		vk::SwapchainKHR m_swapchain;
		std::vector<vk::Framebuffer> m_swapchainFramebuffers;
		std::vector<vk::Image> m_swapchainImages;
		std::vector<vk::ImageView> m_swapchainImageViews;

	public:
		explicit VulkanState(GLFWwindow*);
		~VulkanState();
		BufferInfo& ballVertexBuffer(const uint32_t&);
		const vk::CommandBuffer& commandBuffer(const uint32_t&) const;
		const vk::Fence& fence(const uint32_t&) const;
		const vk::Framebuffer& framebuffer(const uint32_t&) const;
		const vk::Pipeline& graphicsPipeline() const;
		const vk::Queue& graphicsQueue() const;
		const vk::Semaphore& imageAvailableSemaphore(const uint32_t&) const;
		const vk::Semaphore& imageRenderedSemaphore(const uint32_t&) const;
		const vk::Device& logicalDevice() const;
		BufferInfo& opponentVertexBuffer(const uint32_t&);
		const vk::PhysicalDevice& physicalDevice() const;
		BufferInfo& playerVertexBuffer(const uint32_t&);
		const vk::Queue& presentQueue() const;
		const vk::RenderPass& renderPass() const;
		const vk::Extent2D& surfaceExtent() const;
		const vk::SwapchainKHR& swapchain() const;
		uint32_t swapchainImageCount() const;
	};
}