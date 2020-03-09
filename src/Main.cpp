#include "VulkanState.hpp"

#include <iostream>

void draw(const vkPong::VulkanState& vulkanState)
{
	static uint32_t idx = 0;
	vk::Semaphore imageAvailableSemaphore = vulkanState.imageAvailableSemaphore(idx);
	vk::Semaphore imageRenderedSemaphore = vulkanState.imageRenderedSemaphore(idx);
	vk::Fence fence = vulkanState.fence(idx);
	vk::CommandBuffer commandBuffer = vulkanState.commandBuffer(idx);
	vk::Device logicalDevice = vulkanState.logicalDevice();
	vk::SwapchainKHR swapchain = vulkanState.swapchain();
	vk::Queue graphicsQueue = vulkanState.graphicsQueue();
	vk::Queue presentQueue = vulkanState.presentQueue();

	uint32_t imageIndex = logicalDevice.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailableSemaphore, vk::Fence()).value;

	logicalDevice.waitForFences(vk::ArrayProxy<const vk::Fence>(fence), 1, UINT64_MAX);
	logicalDevice.resetFences(vk::ArrayProxy<const vk::Fence>(fence));

	vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

	vk::SubmitInfo submitInfo;
	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&commandBuffer);
	submitInfo.setSignalSemaphoreCount(1);
	submitInfo.setPSignalSemaphores(&imageRenderedSemaphore);
	submitInfo.setWaitSemaphoreCount(1);
	submitInfo.setPWaitSemaphores(&imageAvailableSemaphore);
	submitInfo.setPWaitDstStageMask(&waitDstStageMask);

	graphicsQueue.submit(vk::ArrayProxy<const vk::SubmitInfo>(submitInfo), fence);

	vk::PresentInfoKHR presentInfo;
	presentInfo.setPImageIndices(&imageIndex);
	presentInfo.setSwapchainCount(1);
	presentInfo.setPSwapchains(&swapchain);
	presentInfo.setPWaitSemaphores(&imageRenderedSemaphore);
	presentInfo.setWaitSemaphoreCount(1);

	presentQueue.presentKHR(presentInfo);

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
				vkPong::VulkanState vulkanState(window);

				while (!glfwWindowShouldClose(window))
				{
					glfwPollEvents();
					draw(vulkanState);
				}
			}
			catch (std::exception & e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		glfwTerminate();
	}

	return 0;
}