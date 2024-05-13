#include "vulkanUtils.h"

QueueFamilyIndices VulkanUtils::findQueueFamiliesForDevice(VkPhysicalDevice device) {
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

QueueFamilyIndices VulkanUtils::findQueueFamiliesForSurface(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilyList) {
        // Graphics queue family
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;  // Set graphics family if not already set
        }

        // Presentation support check
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        std::cout << "Queue Family " << i << ": Present Support = " << presentSupport << std::endl;
        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentationFamily = i;  // Set presentation family if not already set
        }

        // Check if a suitable combination is found
        if (indices.isValid()) {
            break;  // Break only if both are found
        }

        i++;
    }

    return indices;
}
