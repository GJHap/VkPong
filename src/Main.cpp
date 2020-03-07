#include <exception>
#include <iostream>

#include <GLFW/glfw3.h>
#include <vulkan\vulkan.h>

#include "VulkanState.h"

void draw(const VulkanState& vulkanState)
{
	static uint32_t idx = 0;
	VkSemaphore imageAvailableSemaphore = vulkanState.imageAvailableSemaphore(idx);
	VkSemaphore imageRenderedSemaphore = vulkanState.imageRenderedSemaphore(idx);
	VkFence fence = vulkanState.fence(idx);
	VkCommandBuffer commandBuffer = vulkanState.commandBuffer(idx);
	VkDevice logicalDevice = vulkanState.logicalDevice();
	VkSwapchainKHR swapchain = vulkanState.swapchain();
	VkQueue graphicsQueue = vulkanState.graphicsQueue();
	VkQueue presentQueue = vulkanState.presentQueue();

	uint32_t imageIndex;
	vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, UINT64_MAX);
	vkResetFences(logicalDevice, 1, &fence);

	VkPipelineStageFlags waitDstStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = {};
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.pNext = nullptr;
	submitInfo.pSignalSemaphores = &imageRenderedSemaphore;
	submitInfo.pWaitDstStageMask = &waitDstStageMask;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pNext = nullptr;
	presentInfo.pResults = nullptr;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pWaitSemaphores = &imageRenderedSemaphore;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount = 1;
	presentInfo.waitSemaphoreCount = 1;

	vkQueuePresentKHR(presentQueue, &presentInfo);

	idx = (idx + 1) % vulkanState.swapchainImageCount();
}

int main(void)
{
	if (glfwInit())
	{
		GLFWwindow* window;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

		if (window != nullptr)
		{
			try
			{
				VulkanState vulkanState(window);

				while (!glfwWindowShouldClose(window))
				{
					glfwPollEvents();
					draw(vulkanState);
				}
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		glfwTerminate();
	}

	return 0;
}