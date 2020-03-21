#include "CommandPoolCreation.hpp"

namespace vkPong
{
	vk::CommandPool createCommandPool(const vk::Device& logicalDevice, const uint32_t& queueFamilyIndex)
	{
		vk::CommandPoolCreateInfo commandPoolCreateInfo;
		commandPoolCreateInfo.setQueueFamilyIndex(queueFamilyIndex);
		commandPoolCreateInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

		return logicalDevice.createCommandPool(commandPoolCreateInfo);
	}
}