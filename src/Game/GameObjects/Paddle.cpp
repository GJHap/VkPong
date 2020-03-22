#include "Paddle.hpp"

namespace vkPong
{
	Paddle::Paddle(const Position& position, const float& verticalIncrement)
		: GameObject(position), m_verticalIncrement{ verticalIncrement }
	{
	}

	const float Paddle::bottom() const { return m_position.y + height * 0.5f; }

	const bool Paddle::collidedWithBall(const Ball& ball) const
	{
		bool result = false;
		if (ball.xDirectionSign() == ballCollisionDirection())
		{
			const Position& ballPosition = ball.position();

			result = ballPosition.x >= left()
				&& ballPosition.x <= right()
				&& ballPosition.y <= bottom()
				&& ballPosition.y >= top();
		}

		return result;
	}

	const float Paddle::left() const { return m_position.x - width * 0.5f; }

	void Paddle::moveUp()
	{
		m_position = { m_position.x, m_position.y - m_verticalIncrement };
	}

	void Paddle::moveDown()
	{
		m_position = { m_position.x, m_position.y + m_verticalIncrement };
	}

	const float Paddle::right() const { return m_position.x + width * 0.5f; }

	const float Paddle::top() const { return m_position.y - height * 0.5f; }

	const uint32_t Paddle::vertexCount() const
	{
		return 4;
	}

	std::vector<VertexData> Paddle::vertexData() const
	{
		const float x_left = left();
		const float x_right = right();
		const float y_bottom = bottom();
		const float y_top = top();

		return
		{
			{ { x_left, y_bottom }, GameObject::color() },
			{ { x_right, y_bottom }, GameObject::color() },
			{ { x_right, y_top },  GameObject::color() },
			{ { x_left, y_top },  GameObject::color() },
		};
	}
}