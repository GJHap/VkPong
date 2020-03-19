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

		static constexpr Color color()
		{
			return { 1.0f, 1.0f, 1.0f };
		}
	};
}