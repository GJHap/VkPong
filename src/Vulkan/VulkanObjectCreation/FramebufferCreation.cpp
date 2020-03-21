#include "FramebufferCreation.hpp"

namespace vkPong
{
	vk::Framebuffer createFramebuffer(const vk::Device& logicalDevice, const vk::ImageView& imageView, const vk::RenderPass& renderPass, const vk::Extent2D& extent)
	{
		vk::FramebufferCreateInfo framebufferCreateInfo;
		framebufferCreateInfo.setAttachmentCount(1);
		framebufferCreateInfo.setPAttachments(&imageView);
		framebufferCreateInfo.setHeight(extent.height);
		framebufferCreateInfo.setWidth(extent.width);
		framebufferCreateInfo.setRenderPass(renderPass);
		framebufferCreateInfo.setLayers(1);

		return logicalDevice.createFramebuffer(framebufferCreateInfo);
	}
}