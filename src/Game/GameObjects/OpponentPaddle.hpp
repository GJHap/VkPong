#pragma once

#include "Paddle.hpp"

namespace vkPong
{
	class OpponentPaddle : public Paddle
	{
	public:
		explicit OpponentPaddle();
		void move(const float& ballYPosition);
	protected:
		const float ballCollisionDirection() const override;
	};
}