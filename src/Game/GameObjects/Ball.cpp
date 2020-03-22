#include "Ball.hpp"
#include "BallVertices.hpp"

#include <cmath>

namespace vkPong
{
	Ball::Ball()
		: GameObject{ {0.0f, 0.0f} }, m_direction{ Direction({0.0005f, 0.0005f}) } {}

	bool Ball::collidedWithPaddle(const Paddle& paddle) const
	{
		const float x_left = paddle.left();
		const float x_right = paddle.right();
		const float y_bottom = paddle.bottom();
		const float y_top = paddle.top();

		return m_position.x >= x_left
			&& m_position.x <= x_right
			&& m_position.y <= y_bottom
			&& m_position.y >= y_top;
	}

	bool Ball::collidedWithWall() const
	{
		return m_position.y <= -1
			|| m_position.y >= 1;
	}

	void Ball::move()
	{
		m_position = { m_position.x + m_direction.x, m_position.y + m_direction.y };
	}

	void Ball::toggleDirectionX()
	{
		m_direction = { -(m_direction.x + direction_increment), m_direction.y + direction_increment };
	}

	void Ball::toggleDirectionY()
	{
		m_direction = { m_direction.x, -m_direction.y };
	}

	const uint32_t Ball::vertexCount() const
	{
		return static_cast<uint32_t>(ballVertexData.size());
	}

	std::vector<VertexData> Ball::vertexData() const
	{
		std::vector<VertexData> vertexData(vertexCount());
		vertexData[0] = { {m_position.x, m_position.y}, GameObject::color() };
		for (size_t i = 0; i < vertexCount() - 1; ++i)
		{
			vertexData[i + 1] = { {static_cast<float>((radius * cos(i) + m_position.x)), static_cast<float>((radius * sin(i)) + m_position.y)}, GameObject::color() };
		}

		return vertexData;
	}
}