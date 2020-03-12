#include "GraphicsPipelineLayoutCreation.hpp"

namespace vkPong
{
	vk::PipelineLayout createGraphicsPipelineLayout(const vk::Device& logicalDevice)
	{
		return logicalDevice.createPipelineLayout(vk::PipelineLayoutCreateInfo());
	}
}