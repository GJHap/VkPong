#include "SwapchainCreation.hpp"

namespace vkPong
{
	vk::SwapchainKHR createSwapchain(const vk::Device& logicalDevice, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::SurfaceFormatKHR& surfaceFormat, const vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

		vk::SwapchainCreateInfoKHR swapchainCreateInfo;
		swapchainCreateInfo.setImageArrayLayers(1);
		swapchainCreateInfo.setImageColorSpace(surfaceFormat.colorSpace);
		swapchainCreateInfo.setImageExtent(surfaceCapabilities.currentExtent);
		swapchainCreateInfo.setImageFormat(surfaceFormat.format);
		swapchainCreateInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
		swapchainCreateInfo.setMinImageCount(surfaceCapabilities.minImageCount);
		swapchainCreateInfo.setPresentMode(presentModes[0]);
		swapchainCreateInfo.setPreTransform(surfaceCapabilities.currentTransform);
		swapchainCreateInfo.setSurface(surface);

		return logicalDevice.createSwapchainKHR(swapchainCreateInfo);
	}
}