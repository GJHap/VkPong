#pragma once

#include "../VulkanState.hpp"
#include "GameObjects\Paddle.hpp"
#include "GameObjects\Ball.hpp"

namespace vkPong
{
	void render(VulkanState&, const Paddle&, const Paddle&, const Ball&);
}