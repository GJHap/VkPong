#include "VertexData.hpp"

namespace vkPong
{
	uint32_t VertexData::positionOffset()
	{
		return 0;
	}

	uint32_t VertexData::colorOffset()
	{
		return sizeof(Position);
	}
}