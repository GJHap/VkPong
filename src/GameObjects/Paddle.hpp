#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Paddle : public GameObject
	{
	private:
	public:
		std::vector<VertexData> vertexData() const override;
	protected:
		explicit Paddle(const Position&);
	private:
		constexpr float height() const { return 0.7f; }
		constexpr float width() const { return 0.1f; }
	};
}