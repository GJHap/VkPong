#include "Paddle.hpp"

namespace vkPong
{
	Paddle::Paddle(const Position& position)
		: GameObject(position)
	{
	}

	void Paddle::moveUp()
	{
		m_position = { m_position.x, m_position.y - verticalIncrement };
	}

	void Paddle::moveDown()
	{
		m_position = { m_position.x, m_position.y + verticalIncrement };
	}

	const uint32_t Paddle::vertexCount() const
	{
		return 4;
	}

	std::vector<VertexData> Paddle::vertexData() const
	{
		const float left = m_position.x - width * 0.5f;
		const float right = m_position.x + width * 0.5f;
		const float bottom = m_position.y + height * 0.5f;
		const float top = m_position.y - height * 0.5f;

		return
		{
			{ { left, bottom }, GameObject::color() },
			{ { right, bottom }, GameObject::color() },
			{ { right, top },  GameObject::color() },
			{ { left, top },  GameObject::color() },
		};
	}
}