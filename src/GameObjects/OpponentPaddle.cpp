#include "OpponentPaddle.hpp"

namespace vkPong
{
	float OpponentPaddle::x_edge_value() const
	{
		return 1.0f;
	}

	int OpponentPaddle::x_sign() const
	{
		return -1;
	}
}