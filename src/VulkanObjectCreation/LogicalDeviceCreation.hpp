#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	struct QueueInfo
	{
		uint32_t queueFamilyIndex;
		vk::Queue queue;
	};

	struct LogicalDeviceInfo
	{
		vk::Device logicalDevice;
		QueueInfo graphicsQueueInfo;
		QueueInfo presentQueueInfo;
	};

	LogicalDeviceInfo createLogicalDevice(const vk::PhysicalDevice&, const vk::SurfaceKHR&);
}