#include "VulkanExtensions.hpp"

vk::DispatchLoaderDynamic getVkCreateDebugReportCallbackEXTDispatchLoader(const vk::Instance& instance)
{
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance.getProcAddr("vkCreateDebugReportCallbackEXT"));
	vk::DispatchLoaderDynamic dispatchLoader;
	dispatchLoader.vkCreateDebugReportCallbackEXT = vkCreateDebugReportCallbackEXT;

	return dispatchLoader;
}

vk::DispatchLoaderDynamic getVkDestroyDebugReportCallbackEXTDispatchLoader(const vk::Instance& instance)
{
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance.getProcAddr("vkDestroyDebugReportCallbackEXT"));

	vk::DispatchLoaderDynamic dispatchLoader;
	dispatchLoader.vkDestroyDebugReportCallbackEXT = vkDestroyDebugReportCallbackEXT;

	return dispatchLoader;
}