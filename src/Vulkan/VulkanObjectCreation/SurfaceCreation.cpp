#include "SurfaceCreation.hpp"

namespace vkPong
{
	vk::SurfaceKHR createSurface(const vk::Instance& instance, GLFWwindow* glfwWindow)
	{
		VkSurfaceKHR surface;
		glfwCreateWindowSurface(instance.operator VkInstance(), glfwWindow, nullptr, &surface);

		return vk::SurfaceKHR(surface);
	}
}