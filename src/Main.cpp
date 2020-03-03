#include <exception>
#include <iostream>

#include <GLFW/glfw3.h>
#include <vulkan\vulkan.h>

#include "VulkanState.h"

int main(void)
{
	if (glfwInit())
	{
		GLFWwindow* window;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

		if (window != nullptr)
		{
			try
			{
				VulkanState vulkanState(window);

				while (!glfwWindowShouldClose(window))
				{
					glfwSwapBuffers(window);
					glfwPollEvents();
				}
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		glfwTerminate();
	}

	return 0;
}