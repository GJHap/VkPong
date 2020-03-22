#include "BufferInfo.hpp"

namespace vkPong
{
	void BufferInfo::free(const vk::Device& logicalDevice)
	{
		logicalDevice.freeMemory(bufferMemory);
		logicalDevice.destroyBuffer(buffer);
	}

	void BufferInfo::setNullHandles()
	{
		buffer = vk::Buffer();
		bufferMemory = vk::DeviceMemory();
	}
}