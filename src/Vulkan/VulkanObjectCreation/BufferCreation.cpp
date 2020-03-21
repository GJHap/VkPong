#include "BufferCreation.hpp"
#include "../../Structs/VertexData.hpp"

namespace vkPong
{
	static int32_t findMemoryTypeIndex(const uint32_t& memoryTypeBits, const vk::PhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties, const vk::MemoryPropertyFlags& memoryPropertyFlags)
	{
		int32_t result = -1;
		for (uint32_t idx = 0; idx < physicalDeviceMemoryProperties.memoryTypeCount; ++idx)
		{
			if ((memoryTypeBits & (1 << idx))
				&& (physicalDeviceMemoryProperties.memoryTypes[idx].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
			{
				result = idx;
			}
		}

		return result;
	}

	static void mapData(const vk::Device& logicalDevice, const vk::DeviceMemory& deviceMemory, const vk::Buffer& buffer, const std::vector<VertexData>& vertexData)
	{
		logicalDevice.bindBufferMemory(buffer, deviceMemory, 0);
		void* data = logicalDevice.mapMemory(deviceMemory, 0, sizeof(vertexData[0]) * vertexData.size());
		memcpy(data, vertexData.data(), sizeof(vertexData[0]) * vertexData.size());
		logicalDevice.unmapMemory(deviceMemory);
	}

	static vk::DeviceMemory allocateBuffer(const vk::Device& logicalDevice, const vk::PhysicalDevice& physicalDevice, const vk::Buffer& buffer)
	{
		vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();
		vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);

		vk::MemoryAllocateInfo memoryAllocateInfo;
		memoryAllocateInfo.setAllocationSize(memoryRequirements.size);
		memoryAllocateInfo.setMemoryTypeIndex(findMemoryTypeIndex(memoryRequirements.memoryTypeBits, physicalDeviceMemoryProperties, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent));

		return logicalDevice.allocateMemory(memoryAllocateInfo);
	}

	BufferInfo createBuffer(const vk::Device& logicalDevice, const vk::PhysicalDevice& physicalDevice, const std::vector<VertexData>& vertexData)
	{
		vk::BufferCreateInfo bufferCreateInfo;
		bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eVertexBuffer);
		bufferCreateInfo.setSize(sizeof(vertexData[0]) * vertexData.size());

		vk::Buffer buffer = logicalDevice.createBuffer(bufferCreateInfo);
		vk::DeviceMemory deviceMemory = allocateBuffer(logicalDevice, physicalDevice, buffer);
		mapData(logicalDevice, deviceMemory, buffer, vertexData);

		BufferInfo bufferInfo;
		bufferInfo.buffer = buffer;
		bufferInfo.bufferMemory = deviceMemory;

		return bufferInfo;
	}
}