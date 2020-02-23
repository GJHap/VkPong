#pragma once

#include <cstdint>

struct VulkanExtensionInfo
{
	uint32_t ExtensionCount;
	const char** ExtensionNames;
};

VulkanExtensionInfo GetVulkanExtensionInfo();
