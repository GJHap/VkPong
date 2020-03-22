#pragma once

#include "GameObjects/Ball.hpp"
#include "GameObjects/PlayerPaddle.hpp"
#include "GameObjects/OpponentPaddle.hpp"
#include "../Vulkan/VulkanState.hpp"

#include "GLFW/glfw3.h"

namespace vkPong
{
	class Game
	{
	private:
		bool m_continueGame{ true };
		bool m_windowIconified{ false };
		PlayerPaddle m_player;
		OpponentPaddle m_opponent;
		Ball m_ball;
		GLFWwindow* m_window;
		VulkanState& m_vulkanState;
	public:
		explicit Game(GLFWwindow*, VulkanState& vulkanState);
		void acceptInput(int);
		void respondToWindowIconified(int);
		void run();
	private:
		void checkBallCollision();
		void pollWhileWindowIconified();
	};
}