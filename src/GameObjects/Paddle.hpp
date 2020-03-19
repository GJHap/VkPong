#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Paddle : public GameObject
	{
	private:
		float m_x;
		float m_y;
	public:
		std::vector<VertexData> vertexData() const override;
	protected:
		Paddle(const float&, const float&);
	private:
		constexpr float height() const { return 0.7f; }
		constexpr float width() const { return 0.1f; }
	};
}