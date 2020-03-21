#include "GameObject.hpp"

namespace vkPong
{
	GameObject::GameObject(const Position& position)
		: m_position{ position }
	{
	}

	const Position& GameObject::position() const { return m_position; }
}