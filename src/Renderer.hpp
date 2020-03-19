#pragma once

#include "VulkanState.hpp"
#include "GameObjects\Paddle.hpp"

namespace vkPong
{
	void render(VulkanState&, const Paddle&, const Paddle&);
}