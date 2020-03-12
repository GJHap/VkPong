#pragma once

#include "../vulkanhpp_include.hpp"

namespace vkPong
{
	vk::DebugReportCallbackEXT createDebugReportCallback(const vk::Instance&);
}