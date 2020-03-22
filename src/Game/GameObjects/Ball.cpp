#include "Ball.hpp"

#include <cmath>

namespace vkPong
{
	static float sign(const float& x)
	{
		float result = 0.0f;
		if (x < 0)
		{
			result = -1.0f;
		}
		else if (x > 0)
		{
			result = 1.0f;
		}

		return result;
	}

	Ball::Ball()
		: GameObject{ {0.0f, 0.0f} }, m_direction{ Direction({0.0005f, 0.0005f}) } {}

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