#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::Fence createFence(const vk::Device&);
}