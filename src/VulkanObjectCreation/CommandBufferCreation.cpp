#include "CommandBufferCreation.hpp"

namespace vkPong
{
	void recordCommandBuffer(const vk::CommandBuffer& commandBuffer,
		const vk::Extent2D& extent,
		const vk::Framebuffer& framebuffer,
		const vk::Pipeline& graphicsPipeline,
		const vk::RenderPass& renderPass,
		const vk::Buffer& playerVertexBuffer,
		const uint32_t& playerVertexCount,
		const vk::Buffer& opponentVertexBuffer,
		const uint32_t& opponentVertexCount,
		const vk::Buffer& ballVertexBuffer,
		const uint32_t& ballVertexCount)
	{
		vk::CommandBufferBeginInfo commandBufferBeginInfo;

		vk::ClearValue clearValue;
		clearValue.setColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

		vk::Rect2D renderArea;
		renderArea.setExtent(extent);

		vk::RenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.setClearValueCount(1);
		renderPassBeginInfo.setPClearValues(&clearValue);
		renderPassBeginInfo.setFramebuffer(framebuffer);
		renderPassBeginInfo.setRenderArea(renderArea);
		renderPassBeginInfo.setRenderPass(renderPass);

		commandBuffer.begin(commandBufferBeginInfo);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
		commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
		commandBuffer.bindVertexBuffers(0, vk::ArrayProxy<const vk::Buffer>{playerVertexBuffer}, vk::ArrayProxy<const vk::DeviceSize>{0});
		commandBuffer.draw(playerVertexCount, 1, 0, 0);
		commandBuffer.bindVertexBuffers(0, vk::ArrayProxy<const vk::Buffer>{opponentVertexBuffer}, vk::ArrayProxy<const vk::DeviceSize>{0});
		commandBuffer.draw(opponentVertexCount, 1, 0, 0);
		commandBuffer.bindVertexBuffers(0, vk::ArrayProxy<const vk::Buffer>{ballVertexBuffer}, vk::ArrayProxy<const vk::DeviceSize>{0});
		commandBuffer.draw(ballVertexCount, 1, 0, 0);
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	vk::CommandBuffer createCommandBuffer(const vk::CommandPool& commandPool, const vk::Device& logicalDevice)
	{
		vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
		commandBufferAllocateInfo.setCommandBufferCount(1);
		commandBufferAllocateInfo.setCommandPool(commandPool);

		return logicalDevice.allocateCommandBuffers(commandBufferAllocateInfo)[0];
	}
}