#include "Renderer.hpp"
#include "../Vulkan/VulkanObjectCreation/BufferCreation.hpp"
#include "../Vulkan/VulkanObjectCreation/CommandBufferCreation.hpp"

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
			bufferInfo.free(logicalDevice);
			bufferInfo = createBuffer(logicalDevice, physicalDevice, vertexData);
		}
	}

	void render(VulkanState& vulkanState, const Paddle& player, const Paddle& opponent, const Ball& ball)
	{
		static uint32_t idx = 0;
		const vk::Device& logicalDevice = vulkanState.logicalDevice();
		const vk::Semaphore& imageAvailableSemaphore = vulkanState.imageAvailableSemaphore(idx);
		const vk::SwapchainKHR& swapchain = vulkanState.swapchain();

		vk::ResultValue<uint32_t> result = logicalDevice.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailableSemaphore, vk::Fence());
		if (result.result == vk::Result::eSuboptimalKHR)
		{
			vulkanState.resetSwapchain();
			idx = 0;
			return;
		}

		const vk::Fence& fence = vulkanState.fence(idx);
		logicalDevice.waitForFences(vk::ArrayProxy<const vk::Fence>(fence), 1, UINT64_MAX);
		logicalDevice.resetFences(vk::ArrayProxy<const vk::Fence>(fence));

		const vk::PhysicalDevice& physicalDevice = vulkanState.physicalDevice();

		BufferInfo& playerBufferInfo = vulkanState.playerVertexBuffer(idx);
		setupVertexBuffer(playerBufferInfo, logicalDevice, physicalDevice, player.vertexData());

		BufferInfo& opponentBufferInfo = vulkanState.opponentVertexBuffer(idx);
		setupVertexBuffer(opponentBufferInfo, logicalDevice, physicalDevice, opponent.vertexData());

		BufferInfo& ballBufferInfo = vulkanState.ballVertexBuffer(idx);
		setupVertexBuffer(ballBufferInfo, logicalDevice, physicalDevice, ball.vertexData());

		const vk::CommandBuffer& playerCommandBuffer = vulkanState.commandBuffer(idx);
		recordCommandBuffer(playerCommandBuffer,
			vulkanState.surfaceExtent(),
			vulkanState.framebuffer(idx),
			vulkanState.graphicsPipeline(),
			vulkanState.renderPass(),
			playerBufferInfo.buffer,
			player.vertexCount(),
			opponentBufferInfo.buffer,
			opponent.vertexCount(),
			ballBufferInfo.buffer,
			ball.vertexCount());

		vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		std::vector<vk::CommandBuffer> commandBuffers{ playerCommandBuffer };

		const vk::Semaphore& imageRenderedSemaphore = vulkanState.imageRenderedSemaphore(idx);
		vk::SubmitInfo submitInfo;
		submitInfo.setCommandBufferCount(static_cast<uint32_t>(commandBuffers.size()));
		submitInfo.setPCommandBuffers(commandBuffers.data());
		submitInfo.setSignalSemaphoreCount(1);
		submitInfo.setPSignalSemaphores(&imageRenderedSemaphore);
		submitInfo.setWaitSemaphoreCount(1);
		submitInfo.setPWaitSemaphores(&imageAvailableSemaphore);
		submitInfo.setPWaitDstStageMask(&waitDstStageMask);

		const vk::Queue& graphicsQueue = vulkanState.graphicsQueue();
		graphicsQueue.submit(vk::ArrayProxy<const vk::SubmitInfo>(submitInfo), fence);

		uint32_t imageIndex = result.value;
		vk::PresentInfoKHR presentInfo;
		presentInfo.setPImageIndices(&imageIndex);
		presentInfo.setSwapchainCount(1);
		presentInfo.setPSwapchains(&swapchain);
		presentInfo.setPWaitSemaphores(&imageRenderedSemaphore);
		presentInfo.setWaitSemaphoreCount(1);

		const vk::Queue& presentQueue = vulkanState.presentQueue();
		presentQueue.presentKHR(presentInfo);

		idx = (idx + 1) % vulkanState.swapchainImageCount();
	}
}