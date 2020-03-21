#pragma once

#include "../vulkanhpp_include.hpp"

#include "GLFW/glfw3.h"

namespace vkPong
{
	vk::SurfaceKHR createSurface(const vk::Instance&, GLFWwindow*);
}