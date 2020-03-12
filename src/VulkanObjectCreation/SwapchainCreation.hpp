#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::SwapchainKHR createSwapchain(const vk::Device&, const vk::PhysicalDevice&, const vk::SurfaceKHR&, const vk::SurfaceFormatKHR&, const vk::SurfaceCapabilitiesKHR&);
}