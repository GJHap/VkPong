#pragma once

#include "Paddle.hpp"

namespace vkPong
{
	class PlayerPaddle : public Paddle
	{
	protected:
		float x_edge_value() const override;
		int x_sign() const override;
	};
}