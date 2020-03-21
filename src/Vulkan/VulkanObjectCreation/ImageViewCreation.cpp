#include "ImageViewCreation.hpp"

namespace vkPong
{
	vk::ImageView createImageView(const vk::Device& logicalDevice, const vk::Image& image, const vk::Format& format)
	{
		vk::ImageSubresourceRange imageSubresourceRange;
		imageSubresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor);
		imageSubresourceRange.setLayerCount(1);
		imageSubresourceRange.setLevelCount(1);

		vk::ImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.setFormat(format);
		imageViewCreateInfo.setImage(image);
		imageViewCreateInfo.setViewType(vk::ImageViewType::e2D);
		imageViewCreateInfo.setSubresourceRange(imageSubresourceRange);

		return logicalDevice.createImageView(imageViewCreateInfo);
	}
}