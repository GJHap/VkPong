#include "InstanceCreation.hpp"

#include "GLFW/glfw3.h"

namespace vkPong
{
	const static std::vector<const char*> INSTANCE_EXTENSIONS
	{
		"VK_EXT_debug_report"
	};

	const static std::vector<const char*> INSTANCE_VALIDATION_LAYERS
	{
		"VK_LAYER_LUNARG_standard_validation"
	};

	static std::vector<const char*> getVulkanInstanceExtensions()
	{
		uint32_t extensionCount;
		const char** ppExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

		std::vector<const char*> extensions(extensionCount);
		std::copy(ppExtensions, ppExtensions + extensionCount, extensions.begin());

		return extensions;
	}

	vk::Instance createInstance()
	{
		std::vector<const char*> instanceExtensions = getVulkanInstanceExtensions();
		std::vector<const char*> enabledLayers;

#ifndef NDEBUG
		enabledLayers = INSTANCE_VALIDATION_LAYERS;
		std::copy(INSTANCE_EXTENSIONS.begin(), INSTANCE_EXTENSIONS.end(), std::back_inserter(instanceExtensions));
#endif

		vk::ApplicationInfo applicationInfo;
		applicationInfo.setPApplicationName("VkPong");
		applicationInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
		applicationInfo.setApiVersion(VK_API_VERSION_1_1);

		vk::InstanceCreateInfo instanceCreateInfo;
		instanceCreateInfo.setPApplicationInfo(&applicationInfo);
		instanceCreateInfo.setEnabledLayerCount(static_cast<uint32_t>(enabledLayers.size()));
		instanceCreateInfo.setPpEnabledLayerNames(enabledLayers.data());
		instanceCreateInfo.setEnabledExtensionCount(static_cast<uint32_t>(instanceExtensions.size()));
		instanceCreateInfo.setPpEnabledExtensionNames(instanceExtensions.data());

		return vk::createInstance(instanceCreateInfo);
	}
}