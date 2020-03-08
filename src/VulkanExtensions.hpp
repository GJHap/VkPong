#pragma once

#ifndef VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#endif

#include <vulkan\vulkan.hpp>

vk::DispatchLoaderDynamic getVkCreateDebugReportCallbackEXTDispatchLoader(const vk::Instance&);
vk::DispatchLoaderDynamic getVkDestroyDebugReportCallbackEXTDispatchLoader(const vk::Instance&);