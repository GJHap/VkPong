#pragma once

#include "GameObjects\Ball.hpp"
#include "GameObjects\Paddle.hpp"
#include "../VulkanState.hpp"

#include "GLFW\glfw3.h"

namespace vkPong
{
	class Game
	{
	private:
		Paddle m_player;
		Paddle m_opponent;
		Ball m_ball;
		GLFWwindow* m_window;
		VulkanState& m_vulkanState;
	public:
		explicit Game(GLFWwindow*, VulkanState& vulkanState);
		void run();
		void acceptInput(int);
	};
}