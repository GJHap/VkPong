#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::CommandBuffer createCommandBuffer(const vk::CommandPool&, const vk::Device&);
	void recordCommandBuffer(const vk::CommandBuffer&,
		const vk::Extent2D&,
		const vk::Framebuffer&,
		const vk::Pipeline&,
		const vk::RenderPass&,
		const vk::Buffer&,
		const uint32_t&);
}