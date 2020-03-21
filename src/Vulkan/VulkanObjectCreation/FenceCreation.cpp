#include "FenceCreation.hpp"

namespace vkPong
{
	vk::Fence createFence(const vk::Device& logicalDevice)
	{
		vk::FenceCreateInfo fenceCreateInfo;
		fenceCreateInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

		return logicalDevice.createFence(fenceCreateInfo);
	}
}