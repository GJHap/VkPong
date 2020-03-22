#include "Ball.hpp"

#include <cmath>

namespace vkPong
{
	static bool collidedWithWall(const float& z)
	{
		return z <= -1
			|| z >= 1;
	}

	static float sign(const float& z)
	{
		float result = 0.0f;
		if (z < 0)
		{
			result = -1.0f;
		}
		else if (z > 0)
		{
			result = 1.0f;
		}

		return result;
	}

	Ball::Ball()
		: GameObject{ default_position }, m_direction{ default_direction } {}

	bool Ball::collidedWithHorizontalWall() const { return collidedWithWall(m_position.x); }

	bool Ball::collidedWithVerticalWall() const { return collidedWithWall(m_position.y); }

	void Ball::move()
	{
		m_position = { m_position.x + m_direction.x, m_position.y + m_direction.y };
	}

	void Ball::reset()
	{
		m_position = default_position;
		m_direction = default_direction;
	}

	void Ball::toggleDirectionX()
	{
		m_direction = { -(m_direction.x + xDirectionSign() * direction_increment), m_direction.y + yDirectionSign() * direction_increment };
	}

	void Ball::toggleDirectionY()
	{
		m_direction = { m_direction.x, -m_direction.y };
	}

	const uint32_t Ball::vertexCount() const
	{
		return 361;
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

	float Ball::xDirectionSign() const { return sign(m_direction.x); }

	float Ball::yDirectionSign() const { return sign(m_direction.y); }
}