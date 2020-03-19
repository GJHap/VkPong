#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Ball : GameObject
	{
	private:
		static std::vector<VertexData> ballVertexData;
	public:
		std::vector<VertexData> vertexData() const override;
	};
}