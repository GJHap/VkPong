#pragma once

#include "../Structs/Position.hpp"
#include "../Structs/VertexData.hpp"

#include <vector>

namespace vkPong
{
	class GameObject
	{
	public:
		virtual std::vector<VertexData> vertexData() const = 0;
		const Color color() const;
	};
}