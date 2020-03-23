#include "Vulkan/VulkanState.hpp"
#include "Game/Game.hpp"

#include <iostream>

int main(void)
{
	if (glfwInit())
	{
		constexpr int windowHeight = 1080;
		constexpr int windowWidth = 1920;

		GLFWwindow* window;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(windowWidth, windowHeight, "VkPong", NULL, NULL);
		glfwSetWindowSizeLimits(window, GLFW_DONT_CARE, GLFW_DONT_CARE, windowWidth, windowHeight);

		if (window != nullptr)
		{
			try
			{
				vkPong::VulkanState vulkanState(window);
				vkPong::Game game(window, vulkanState);

				game.run();
			}
			catch (std::exception & e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		glfwTerminate();
	}

	return 0;
}