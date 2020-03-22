#include "Game.hpp"
#include "Renderer.hpp"

namespace vkPong
{
	static void checkBallCollision(Ball& ball, const Paddle& player, const Paddle& opponent)
	{
		if (ball.collidedWithPaddle(player) || ball.collidedWithPaddle(opponent))
		{
			ball.toggleDirectionX();
		}
		else if (ball.collidedWithWall())
		{
			ball.toggleDirectionY();
		}
	}

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
			checkBallCollision(m_ball, m_player, m_opponent);
			m_opponent.move(m_ball);
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