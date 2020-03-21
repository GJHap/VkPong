#include "OpponentPaddle.hpp"

#include "Ball.hpp"

namespace vkPong
{
	OpponentPaddle::OpponentPaddle()
		: Paddle({ 0.95f, 0.65f })
	{
	}

	void OpponentPaddle::move(const Ball& ball)
	{
		if (m_position.y < ball.position().y)
		{
			moveDown();
		}
		else if (m_position.y > ball.position().y)
		{
			moveUp();
		}
	}
}