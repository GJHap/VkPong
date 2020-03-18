#pragma once

#include "../vulkanhpp_include.hpp"
#include "../Structs/BufferInfo.hpp"
#include "../Structs/VertexData.hpp"

namespace vkPong
{
	BufferInfo createBuffer(const vk::Device&, const vk::PhysicalDevice&, const std::vector<VertexData>&);
}