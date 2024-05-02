// VulkanUtils.h
#pragma once

#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentationFamily = -1;

    bool isValid() const {
        return graphicsFamily >= 0 && presentationFamily >=0;
    }
};

namespace VulkanUtils {
    // Finds queue families without considering presentation support
    QueueFamilyIndices findQueueFamiliesForDevice(VkPhysicalDevice device);

    // Finds queue families with presentation support considered
    QueueFamilyIndices findQueueFamiliesForSurface(VkPhysicalDevice device, VkSurfaceKHR surface);
}
