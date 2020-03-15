#pragma once

#include "vulkanhpp_include.hpp"
#include "BufferInfo.hpp"

#include <GLFW\glfw3.h>

namespace vkPong
{
	class VulkanState
	{
	private:
		BufferInfo m_bufferInfo;
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
		vk::SwapchainKHR m_swapchain;
		std::vector<vk::Framebuffer> m_swapchainFramebuffers;
		std::vector<vk::Image> m_swapchainImages;
		std::vector<vk::ImageView> m_swapchainImageViews;
		vk::Device m_logicalDevice;
		vk::PhysicalDevice m_physicalDevice;
		vk::Queue m_presentQueue;
		vk::RenderPass m_renderPass;
		vk::SurfaceKHR m_surface;

	public:
		explicit VulkanState(GLFWwindow*);
		~VulkanState();
		vk::CommandBuffer commandBuffer(const uint32_t&) const;
		vk::Fence fence(const uint32_t&) const;
		vk::Queue graphicsQueue() const;
		vk::Semaphore imageAvailableSemaphore(const uint32_t&) const;
		vk::Semaphore imageRenderedSemaphore(const uint32_t&) const;
		vk::Device logicalDevice() const;
		vk::Queue presentQueue() const;
		vk::SwapchainKHR swapchain() const;
		uint32_t swapchainImageCount() const;
	};
}