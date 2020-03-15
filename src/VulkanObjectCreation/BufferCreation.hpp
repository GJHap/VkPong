#pragma once

#include "../vulkanhpp_include.hpp"
#include "../BufferInfo.hpp"

namespace vkPong
{
	BufferInfo createBuffer(const vk::Device&, const vk::PhysicalDevice&);
}