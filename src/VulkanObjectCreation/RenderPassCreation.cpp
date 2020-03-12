#include "RenderPassCreation.hpp"

namespace vkPong
{
	vk::RenderPass createRenderPass(const vk::Device& logicalDevice, const vk::Format& format)
	{
		vk::AttachmentDescription attachmentDescription;
		attachmentDescription.setFormat(format);
		attachmentDescription.setLoadOp(vk::AttachmentLoadOp::eDontCare);
		attachmentDescription.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
		attachmentDescription.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
		attachmentDescription.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

		vk::AttachmentReference attachmentReference;
		attachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		vk::SubpassDescription subpassDescription;
		subpassDescription.setColorAttachmentCount(1);
		subpassDescription.setPColorAttachments(&attachmentReference);

		vk::RenderPassCreateInfo renderPassCreateInfo;
		renderPassCreateInfo.setAttachmentCount(1);
		renderPassCreateInfo.setPAttachments(&attachmentDescription);
		renderPassCreateInfo.setSubpassCount(1);
		renderPassCreateInfo.setPSubpasses(&subpassDescription);

		return logicalDevice.createRenderPass(renderPassCreateInfo);
	}
}