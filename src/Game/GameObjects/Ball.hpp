#pragma once

#include "GameObject.hpp"
#include "../../Structs/Direction.hpp"

namespace vkPong
{
	class Ball : public GameObject
	{
	private:
		Direction m_direction;
		constexpr static Direction default_direction{ 0.0005f, 0.0005f };
		constexpr static Position default_position{ 0.0f, 0.0f };
		constexpr static float direction_increment{ 0.00005f };
		constexpr static double radius{ 0.1 };
	public:
		explicit Ball();
		bool collidedWithHorizontalWall() const;
		bool collidedWithVerticalWall() const;
		void move();
		void reset();
		void toggleDirectionX();
		void toggleDirectionY();
		const uint32_t vertexCount() const override;
		std::vector<VertexData> vertexData() const override;
		float xDirectionSign() const;
	private:
		float yDirectionSign() const;
	};
}