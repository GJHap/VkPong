#pragma once

#include "../../Structs/Position.hpp"
#include "../../Structs/VertexData.hpp"

#include <vector>

namespace vkPong
{
	class GameObject
	{
	protected:
		Position m_position;
	public:
		explicit GameObject(const Position&);
		virtual const uint32_t vertexCount() const = 0;
		virtual std::vector<VertexData> vertexData() const = 0;

		static constexpr Color color()
		{
			return { 1.0f, 1.0f, 1.0f };
		}
	};
}