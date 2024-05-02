// VulkanUtils.h
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentationFamily = -1;

    bool isValid() const {
        return graphicsFamily >= 0 && presentationFamily >=0;
    }
};

namespace VulkanUtils {
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
}