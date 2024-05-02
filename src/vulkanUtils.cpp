#include "vulkanUtils.h"

QueueFamilyIndices VulkanUtils::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    // Get all queue family property info for the given device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilyList) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (indices.isValid()) {
            break;
        }

        i++;
    }

    return indices;
}

QueueFamilyIndices VulkanUtils::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    // Get all queue family property info for the given device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilyList) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        // Check if queue family supports presentation.
        VkBool32 presentationSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);
        
        // Check if queue is presentation type (can be both graphics and presentation)
        if (queueFamily.queueCount >0 && presentationSupport)
        {
            indices.presentationFamily = i;
        }
        

        if (indices.isValid()) {
            break;
        }

        i++;
    }

    return indices;
}