#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::ImageView createImageView(const vk::Device&, const vk::Image&, const vk::Format&);
}