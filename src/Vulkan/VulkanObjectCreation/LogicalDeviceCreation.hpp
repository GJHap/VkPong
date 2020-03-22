#pragma once

#include "../vulkanhpp_include.hpp"
#include "../QueueInfo.hpp"

namespace vkPong
{
	struct LogicalDeviceInfo
	{
		vk::Device logicalDevice;
		QueueInfo graphicsQueueInfo;
		QueueInfo presentQueueInfo;
	};

	LogicalDeviceInfo createLogicalDevice(const vk::PhysicalDevice&, const vk::SurfaceKHR&);
}