#pragma once

#include "GameObject.hpp"
#include "Ball.hpp"

namespace vkPong
{
	class Paddle : public GameObject
	{
	private:
		constexpr static float height{ 0.7f };
		constexpr static float width{ 0.1f };
		const float m_verticalIncrement;
	public:
		const virtual bool collidedWithBall(const Ball&) const;
		void moveDown();
		void moveUp();
		const uint32_t vertexCount() const override;
		std::vector<VertexData> vertexData() const override;
	protected:
		explicit Paddle(const Position&, const float&);
		const virtual float ballCollisionDirection() const = 0;
	private:
		const float bottom() const;
		const float left() const;
		const float right() const;
		const float top() const;
	};
}