#include "Paddle.hpp"

namespace vkPong
{
	Paddle::Paddle(const float& x, const float& y)
	{
		m_x = x;
		m_y = y;
	}

	std::vector<VertexData> Paddle::vertexData() const
	{
		const float x_left = m_x - width() * 0.5f;
		const float x_right = m_x + width() * 0.5f;
		const float y_bottom = m_y + height() * 0.5f;
		const float y_top = m_y - height() * 0.5f;

		return
		{
			{ { x_left, y_bottom }, GameObject::color() },
			{ { x_right, y_bottom }, GameObject::color() },
			{ { x_right, y_top },  GameObject::color() },
			{ { x_left, y_top },  GameObject::color() },
		};
	}
}