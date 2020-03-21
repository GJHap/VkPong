#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Ball : GameObject
	{
	public:
		explicit Ball();
		const uint32_t vertexCount() const override;
		std::vector<VertexData> vertexData() const override;
	};
}