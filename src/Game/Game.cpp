#include "Game.hpp"

#include "GameObjects/PlayerPaddle.hpp"
#include "GameObjects/OpponentPaddle.hpp"

#include "Renderer.hpp"

namespace vkPong
{
	static void keyCallback(GLFWwindow* window, int key, int, int, int)
	{
		Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
		game->acceptInput(key);
	}

	Game::Game(GLFWwindow* window, VulkanState& vulkanState)
		: m_player{ PlayerPaddle() }, m_opponent{ OpponentPaddle() }, m_ball{ Ball() }, m_window{ window }, m_vulkanState{ vulkanState }
	{}

	void Game::run()
	{
		glfwSetWindowUserPointer(m_window, this);
		glfwSetKeyCallback(m_window, keyCallback);

		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
			m_ball.move();
			vkPong::render(m_vulkanState, m_player, m_opponent, m_ball);
		}
	}

	void Game::acceptInput(int key)
	{
		if (key == GLFW_KEY_UP)
		{
			m_player.moveUp();
		}
		else if (key == GLFW_KEY_DOWN)
		{
			m_player.moveDown();
		}
	}
}