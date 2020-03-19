#include "Paddle.hpp"

namespace vkPong
{
	std::vector<VertexData> Paddle::vertexData() const
	{
		return
		{
			{ { x_edge_value(), y_edge_value() }, GameObject::color() },
			{ { x_edge_value() + x_sign() * width(), y_edge_value() }, GameObject::color() },
			{ { x_edge_value() + x_sign() * width(), y_edge_value() - height() },  GameObject::color() },
			{ { x_edge_value(), y_edge_value() - height() },  GameObject::color() },
		};
	}

	float Paddle::y_edge_value() const
	{
		return 1.0f;
	}

	float Paddle::height() const
	{
		return 0.7f;
	}

	float Paddle::width() const
	{
		return 0.1f;
	}
}