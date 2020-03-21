#pragma once

#include "Paddle.hpp"

#include "Ball.hpp"

namespace vkPong
{
	class OpponentPaddle : public Paddle
	{
	public:
		explicit OpponentPaddle();
		void move(const Ball&);
	};
}