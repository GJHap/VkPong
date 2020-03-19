#pragma once

#include "Paddle.hpp"

namespace vkPong
{
	class OpponentPaddle : public Paddle
	{
	protected:
		float x_edge_value() const override;
		int x_sign() const override;
	};
}