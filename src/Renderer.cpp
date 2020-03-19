#include "Renderer.hpp"

#include "VulkanObjectCreation\VulkanObjectCreation.hpp"

namespace vkPong
{
	static void setupVertexBuffer(BufferInfo& bufferInfo,
		const vk::Device& logicalDevice,
		const vk::PhysicalDevice& physicalDevice,
		const std::vector<VertexData>& vertexData)
	{
		if (bufferInfo.buffer.operator VkBuffer() == VK_NULL_HANDLE)
		{
			bufferInfo = createBuffer(logicalDevice, physicalDevice, vertexData);
		}
		else
		{
			logicalDevice.freeMemory(bufferInfo.bufferMemory);
			logicalDevice.destroyBuffer(bufferInfo.buffer);
			bufferInfo = createBuffer(logicalDevice, physicalDevice, vertexData);
		}
	}

	void render(VulkanState& vulkanState, const Paddle& player, const Paddle& opponent)
	{
		static uint32_t idx = 0;
		const vk::Semaphore& imageAvailableSemaphore = vulkanState.imageAvailableSemaphore(idx);
		const vk::Semaphore& imageRenderedSemaphore = vulkanState.imageRenderedSemaphore(idx);
		const vk::Fence& fence = vulkanState.fence(idx);
		const vk::Device& logicalDevice = vulkanState.logicalDevice();
		const vk::PhysicalDevice& physicalDevice = vulkanState.physicalDevice();
		const vk::SwapchainKHR& swapchain = vulkanState.swapchain();
		const vk::Queue& graphicsQueue = vulkanState.graphicsQueue();
		const vk::Queue& presentQueue = vulkanState.presentQueue();
		const vk::Framebuffer& framebuffer = vulkanState.framebuffer(idx);
		const vk::Pipeline& graphicsPipeline = vulkanState.graphicsPipeline();
		const vk::RenderPass& renderPass = vulkanState.renderPass();
		const vk::Extent2D& surfaceExtent = vulkanState.surfaceExtent();
		const vk::CommandBuffer& playerCommandBuffer = vulkanState.playerCommandBuffer(idx);
		BufferInfo& playerBufferInfo = vulkanState.playerVertexBuffer(idx);
		const vk::CommandBuffer& opponentCommandBuffer = vulkanState.opponentCommandBuffer(idx);
		BufferInfo& opponentBufferInfo = vulkanState.opponentVertexBuffer(idx);

		uint32_t imageIndex = logicalDevice.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailableSemaphore, vk::Fence()).value;

		logicalDevice.waitForFences(vk::ArrayProxy<const vk::Fence>(fence), 1, UINT64_MAX);
		logicalDevice.resetFences(vk::ArrayProxy<const vk::Fence>(fence));

		setupVertexBuffer(playerBufferInfo, logicalDevice, physicalDevice, player.vertexData());
		recordCommandBuffer(playerCommandBuffer, surfaceExtent, framebuffer, graphicsPipeline, renderPass, playerBufferInfo.buffer, static_cast<uint32_t>(player.vertexData().size()));

		setupVertexBuffer(opponentBufferInfo, logicalDevice, physicalDevice, opponent.vertexData());
		recordCommandBuffer(opponentCommandBuffer, surfaceExtent, framebuffer, graphicsPipeline, renderPass, opponentBufferInfo.buffer, static_cast<uint32_t>(opponent.vertexData().size()));

		vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		std::vector<vk::CommandBuffer> commandBuffers{ playerCommandBuffer, opponentCommandBuffer };

		vk::SubmitInfo submitInfo;
		submitInfo.setCommandBufferCount(static_cast<uint32_t>(commandBuffers.size()));
		submitInfo.setPCommandBuffers(commandBuffers.data());
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
}