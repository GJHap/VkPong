#pragma once

#include "vulkanhpp_include.hpp"

namespace vkPong
{
	vk::DispatchLoaderDynamic getVkCreateDebugReportCallbackEXTDispatchLoader(const vk::Instance&);
	vk::DispatchLoaderDynamic getVkDestroyDebugReportCallbackEXTDispatchLoader(const vk::Instance&);
}