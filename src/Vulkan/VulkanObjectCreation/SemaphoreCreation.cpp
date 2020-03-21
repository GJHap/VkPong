#include "SemaphoreCreation.hpp"

namespace vkPong
{
	vk::Semaphore createSemaphore(const vk::Device& logicalDevice)
	{
		return logicalDevice.createSemaphore(vk::SemaphoreCreateInfo());
	}
}