#include "LogicalDeviceCreation.hpp"

namespace vkPong
{
	const static std::vector<const char*> DEVICE_EXTENSIONS
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct QueueFamilyIndexInfo
	{
		uint32_t graphicsQueueFamilyIndex;
		uint32_t presentQueueFamilyIndex;
	};

	static QueueFamilyIndexInfo getVulkanQueueInfo(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
	{
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

		QueueFamilyIndexInfo queueInfo = {};
		for (decltype(queueFamilyProperties.size()) i = 0; i < queueFamilyProperties.size(); ++i)
		{
			if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics)
			{
				queueInfo.graphicsQueueFamilyIndex = static_cast<uint32_t>(i);
			}

			if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface))
			{
				queueInfo.presentQueueFamilyIndex = static_cast<uint32_t>(i);
			}
		}

		return queueInfo;
	}

	static vk::DeviceQueueCreateInfo createDeviceQueueCreateInfo(const float& queuePriority, const uint32_t& queueFamilyIndex)
	{
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo;
		deviceQueueCreateInfo.setPQueuePriorities(&queuePriority);
		deviceQueueCreateInfo.setQueueFamilyIndex(queueFamilyIndex);
		deviceQueueCreateInfo.setQueueCount(1);

		return deviceQueueCreateInfo;
	}

	static std::vector<vk::DeviceQueueCreateInfo> getDeviceQueueCreateInfos(const vk::DeviceQueueCreateInfo& graphicsQueueCreateInfo, const vk::DeviceQueueCreateInfo& presentQueueCreateInfo)
	{
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

		if (graphicsQueueCreateInfo.queueFamilyIndex == presentQueueCreateInfo.queueFamilyIndex)
		{
			queueCreateInfos = { graphicsQueueCreateInfo };
		}
		else
		{
			queueCreateInfos = { graphicsQueueCreateInfo, presentQueueCreateInfo };
		}

		return queueCreateInfos;
	}

	LogicalDeviceInfo createLogicalDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
	{
		const float queuePriority = 1.0f;
		QueueFamilyIndexInfo queueFamilyIndexInfo = getVulkanQueueInfo(physicalDevice, surface);
		std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos = getDeviceQueueCreateInfos(createDeviceQueueCreateInfo(queuePriority, queueFamilyIndexInfo.graphicsQueueFamilyIndex), createDeviceQueueCreateInfo(queuePriority, queueFamilyIndexInfo.presentQueueFamilyIndex));

		vk::DeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.setQueueCreateInfoCount(1);
		deviceCreateInfo.setPQueueCreateInfos(deviceQueueCreateInfos.data());
		deviceCreateInfo.setEnabledExtensionCount(static_cast<uint32_t>(DEVICE_EXTENSIONS.size()));
		deviceCreateInfo.setPpEnabledExtensionNames(DEVICE_EXTENSIONS.data());

		vk::Device logicalDevice = physicalDevice.createDevice(deviceCreateInfo);
		vk::Queue graphicsQueue = logicalDevice.getQueue(queueFamilyIndexInfo.graphicsQueueFamilyIndex, 0);
		vk::Queue presentQueue = logicalDevice.getQueue(queueFamilyIndexInfo.presentQueueFamilyIndex, 0);

		QueueInfo graphicsInfo = { queueFamilyIndexInfo.graphicsQueueFamilyIndex, graphicsQueue };
		QueueInfo presentInfo = { queueFamilyIndexInfo.presentQueueFamilyIndex, presentQueue };

		return { logicalDevice, graphicsInfo, presentInfo };
	}
}