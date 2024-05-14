#include "vulkanUtils.h"

namespace VulkanUtils{

    QueueFamilyIndices storedIndices;

    QueueFamilyIndices findQueueFamiliesForDevice(VkPhysicalDevice device) {
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

    QueueFamilyIndices findQueueFamiliesForSurface(VkPhysicalDevice device, VkSurfaceKHR surface) {
        // If the stored indices are valid, return them directly
        if (storedIndices.isValid()) {
            std::cout << "Returning stored queue family indices." << std::endl;
            return storedIndices;
        }

        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        if (queueFamilyCount == 0) {
            std::cerr << "No queue families found." << std::endl;
            return indices;
        }

        std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilyList) {
            // Graphics queue family
            if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                indices.graphicsFamily = i;
            }

            // Presentation support check
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.presentationFamily = i;
            }

            // Check if a suitable combination is found
            if (indices.isValid()) {
                std::cout << "Queue Family " << i << ": Graphics and Presentation Support found." << std::endl;
                storedIndices = indices; // Store the valid indices
                break;
            }

            i++;
        }

        return indices;
    }
}
