#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Paddle : public GameObject
	{
	public:
		std::vector<VertexData> vertexData() const override;
	};
}