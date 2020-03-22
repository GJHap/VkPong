#pragma once

#include "vulkanhpp_include.hpp"

namespace vkPong
{
	struct QueueInfo
	{
		uint32_t queueFamilyIndex;
		vk::Queue queue;
	};
}