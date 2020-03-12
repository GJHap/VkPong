#include "DebugReportbackCreation.hpp"
#include "../VulkanExtensions.hpp"

#include <iostream>

namespace vkPong
{
#ifdef NDEBUG
	const bool enableValidation = false;
#else
	const bool enableValidation = true;
#endif

	static VkBool32 debugCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char* pMessage, void*)
	{
		std::cerr << pMessage << "\n\n";
		return VK_FALSE;
	}

	vk::DebugReportCallbackEXT createDebugReportCallback(const vk::Instance& instance)
	{
		vk::DebugReportCallbackEXT debugReportCallback;

		if (enableValidation)
		{
			vk::DebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;
			debugReportCallbackCreateInfo.setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eDebug);
			debugReportCallbackCreateInfo.setPfnCallback(&debugCallback);

			debugReportCallback = instance.createDebugReportCallbackEXT(debugReportCallbackCreateInfo, nullptr, getVkCreateDebugReportCallbackEXTDispatchLoader(instance));
		}

		return debugReportCallback;
	}
}