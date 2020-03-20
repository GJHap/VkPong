#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Ball : GameObject
	{
	public:
		explicit Ball();
		std::vector<VertexData> vertexData() const override;
	};
}