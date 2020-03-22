#include "Game.hpp"
#include "Renderer.hpp"

namespace vkPong
{
	static void keyCallback(GLFWwindow* window, int key, int, int, int)
	{
		Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
		game->acceptInput(key);
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
		else if (key == GLFW_KEY_Q)
		{
			m_continueGame = false;
		}
	}

	void Game::checkBallCollision()
	{
		if (m_player.collidedWithBall(m_ball) || m_opponent.collidedWithBall(m_ball))
		{
			m_ball.toggleDirectionX();
		}
		else if (m_ball.collidedWithVerticalWall())
		{
			m_ball.toggleDirectionY();
		}
		else if (m_ball.collidedWithHorizontalWall())
		{
			m_ball.reset();
		}
	}

	Game::Game(GLFWwindow* window, VulkanState& vulkanState)
		: m_player{ PlayerPaddle() }, m_opponent{ OpponentPaddle() }, m_ball{ Ball() }, m_window{ window }, m_vulkanState{ vulkanState }
	{}

	void Game::run()
	{
		glfwSetWindowUserPointer(m_window, this);
		glfwSetKeyCallback(m_window, keyCallback);

		while (!glfwWindowShouldClose(m_window) && m_continueGame)
		{
			glfwPollEvents();
			checkBallCollision();
			m_opponent.move(m_ball.position().y);
			m_ball.move();
			vkPong::render(m_vulkanState, m_player, m_opponent, m_ball);
		}
	}
}