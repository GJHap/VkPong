#include "PlayerPaddle.hpp"

namespace vkPong
{
	float PlayerPaddle::x_edge_value() const
	{
		return -1.0f;
	}

	int PlayerPaddle::x_sign() const
	{
		return 1;
	}
}