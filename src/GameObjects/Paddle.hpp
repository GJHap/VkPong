#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Paddle : public GameObject
	{
	public:
		std::vector<VertexData> vertexData() const override;
	protected:
		virtual float x_edge_value() const = 0;
		virtual int x_sign() const = 0;
		float y_edge_value() const;
		float height() const;
		float width() const;
	};
}