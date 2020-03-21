#include "Vulkan/VulkanState.hpp"
#include "Game/Game.hpp"

#include <iostream>

int main(void)
{
	if (glfwInit())
	{
		GLFWwindow* window;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(640, 480, "VkPong", NULL, NULL);

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