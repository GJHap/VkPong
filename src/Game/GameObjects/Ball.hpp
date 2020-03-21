#pragma once

#include "GameObject.hpp"
#include "../../Structs/Direction.hpp"

namespace vkPong
{
	class Ball : GameObject
	{
	private:
		Direction m_direction;
	public:
		explicit Ball();
		const uint32_t vertexCount() const override;
		std::vector<VertexData> vertexData() const override;
		void move();
	};
}