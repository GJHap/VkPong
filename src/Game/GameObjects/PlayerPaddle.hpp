#pragma once

#include "Paddle.hpp"

namespace vkPong
{
	class PlayerPaddle : public Paddle
	{
	public:
		explicit PlayerPaddle();
	protected:
		const float ballCollisionDirection() const override;
	};
}