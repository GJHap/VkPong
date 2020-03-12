#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::Framebuffer createFramebuffer(const vk::Device&, const vk::ImageView&, const vk::RenderPass&, const vk::Extent2D&);
}