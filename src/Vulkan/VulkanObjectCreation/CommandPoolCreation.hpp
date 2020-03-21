#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::CommandPool createCommandPool(const vk::Device&, const uint32_t&);
}