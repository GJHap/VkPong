#include "CommandBufferCreation.hpp"

namespace vkPong
{
	static void recordCommandBuffer(const vk::CommandBuffer& commandBuffer, const vk::Extent2D& extent, const vk::Framebuffer& framebuffer, const vk::Pipeline& graphicsPipeline, const vk::RenderPass& renderPass)
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
		commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
		commandBuffer.draw(3, 1, 0, 0);
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	vk::CommandBuffer createCommandBuffer(const vk::CommandPool& commandPool,
		const vk::Device& logicalDevice,
		const vk::Extent2D& extent,
		const vk::Framebuffer& framebuffer,
		const vk::Pipeline& graphicsPipeline,
		const vk::RenderPass& renderPass)
	{
		vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
		commandBufferAllocateInfo.setCommandBufferCount(1);
		commandBufferAllocateInfo.setCommandPool(commandPool);

		vk::CommandBuffer commandBuffer = logicalDevice.allocateCommandBuffers(commandBufferAllocateInfo)[0];

		recordCommandBuffer(commandBuffer, extent, framebuffer, graphicsPipeline, renderPass);

		return commandBuffer;
	}
}