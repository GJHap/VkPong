#pragma once

#include "Position.hpp"
#include "Color.hpp"

#include <cstdint>

namespace vkPong
{
	struct VertexData
	{
		Position position;
		Color color;

		static uint32_t positionOffset();
		static uint32_t colorOffset();
	};
}