#pragma once

#include "vulkanhpp_include.hpp"

namespace vkPong
{
	struct BufferInfo
	{
		vk::Buffer buffer;
		vk::DeviceMemory bufferMemory;
	};
}