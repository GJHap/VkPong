#include "VulkanState.hpp"
#include "GameObjects\Paddle.hpp"
#include "Structs\BufferInfo.hpp"
#include "Structs\VertexData.hpp"
#include "Renderer.hpp"

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
				vkPong::Paddle player;

				while (!glfwWindowShouldClose(window))
				{
					glfwPollEvents();
					vkPong::render(vulkanState, player);
				}
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