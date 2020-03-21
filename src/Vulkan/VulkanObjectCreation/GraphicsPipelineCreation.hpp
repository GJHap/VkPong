#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::Pipeline createGraphicsPipeline(const vk::PipelineLayout&, const vk::Device&, const vk::RenderPass&, const vk::Extent2D&);
}