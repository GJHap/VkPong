#pragma once

#include <vector>

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

class VulkanState
{
private:
	std::vector<VkCommandBuffer> m_commandBuffers;
	VkCommandPool m_commandPool;
	VkDebugReportCallbackEXT m_debugReportCallback;
	VkPipeline m_graphicsPipeline;
	VkPipelineLayout m_graphicsPipelineLayout;
	VkQueue m_graphicsQueue;
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_imageRenderedSemaphores;
	VkInstance m_instance;
	VkSwapchainKHR m_swapChain;
	std::vector<VkFramebuffer> m_swapchainFramebuffers;
	std::vector<VkImage> m_swapchainImages;
	std::vector<VkImageView> m_swapchainImageViews;
	VkDevice m_logicalDevice;
	VkPhysicalDevice m_physicalDevice;
	VkQueue m_presentQueue;
	VkRenderPass m_renderPass;
	VkSurfaceKHR m_surface;

public:
	VulkanState(GLFWwindow*);
	~VulkanState();
};
