#include "PlayerPaddle.hpp"

namespace vkPong
{
	PlayerPaddle::PlayerPaddle()
		: Paddle({ -0.95f, 0.65f }, 0.1f)
	{
	}

	const float PlayerPaddle::ballCollisionDirection() const
	{
		return -1.0f;
	}
}