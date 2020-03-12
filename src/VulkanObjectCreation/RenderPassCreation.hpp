#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::RenderPass createRenderPass(const vk::Device&, const vk::Format&);
}