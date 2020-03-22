#include "OpponentPaddle.hpp"

namespace vkPong
{
	OpponentPaddle::OpponentPaddle()
		: Paddle({ 0.95f, 0.65f }, 0.01f)
	{
	}

	const float OpponentPaddle::ballCollisionDirection() const
	{
		return 1.0f;
	}

	void OpponentPaddle::move(const float& ballYPosition)
	{
		if (m_position.y < ballYPosition)
		{
			moveDown();
		}
		else if (m_position.y > ballYPosition)
		{
			moveUp();
		}
	}
}