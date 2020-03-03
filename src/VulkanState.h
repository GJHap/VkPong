#pragma once

#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>

class VulkanState
{
private:
	VkCommandBuffer m_commandBuffer;
	VkCommandPool m_commandPool;
	VkDebugReportCallbackEXT m_debugReportCallback;
	VkPipeline m_graphicsPipeline;
	VkPipelineLayout m_graphicsPipelineLayout;
	VkQueue m_graphicsQueue;
	VkImageView m_imageView;
	VkInstance m_instance;
	VkSwapchainKHR m_swapChain;
	VkImage m_swapChainImage;
	VkDevice m_logicalDevice;
	VkPhysicalDevice m_physicalDevice;
	VkQueue m_presentQueue;
	VkRenderPass m_renderPass;
	VkSurfaceKHR m_surface;

public:
	VulkanState(GLFWwindow*);
	~VulkanState();
};
