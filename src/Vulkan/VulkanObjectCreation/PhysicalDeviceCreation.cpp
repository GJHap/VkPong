#include "PhysicalDeviceCreation.hpp"

namespace vkPong
{
	vk::PhysicalDevice createPhysicalDevice(const vk::Instance& instance)
	{
		return instance.enumeratePhysicalDevices()[0];
	}
}