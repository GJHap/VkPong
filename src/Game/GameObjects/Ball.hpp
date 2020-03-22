#pragma once

#include "GameObject.hpp"
#include "Paddle.hpp"
#include "../../Structs/Direction.hpp"

namespace vkPong
{
	class Ball : public GameObject
	{
	private:
		Direction m_direction;
		constexpr static float direction_increment{ 0.00001f };
		constexpr static double radius{ 0.1 };
	public:
		explicit Ball();
		bool collidedWithPaddle(const Paddle&) const;
		bool collidedWithWall() const;
		void move();
		void toggleDirectionX();
		void toggleDirectionY();
		const uint32_t vertexCount() const override;
		std::vector<VertexData> vertexData() const override;
	};
}