#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::Semaphore createSemaphore(const vk::Device&);
}