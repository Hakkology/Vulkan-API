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
        std::cout << "Finding queue families for surface." << std::endl;

        if (storedIndices.isValid()) {
            
            std::cout << "Using stored queue family indices." << std::endl;
            std::cout << "Stored Graphics Family Index: " << storedIndices.graphicsFamily << std::endl;
            std::cout << "Stored Presentation Family Index: " << storedIndices.presentationFamily << std::endl;
            return storedIndices;
        }

        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::cout << "Number of queue families: " << queueFamilyCount << std::endl;

        if (queueFamilyCount == 0) {
            std::cerr << "No queue families found." << std::endl;
            return indices;
        }

        std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilyList) {
            std::cout << "Checking queue family " << i << ": Queue Count = " << queueFamily.queueCount
                    << ", Queue Flags = " << queueFamily.queueFlags << std::endl;

            if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                indices.graphicsFamily = i;
                std::cout << "Graphics family set to " << i << std::endl;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            std::cout << "Presentation support for family " << i << ": " << (presentSupport ? "Yes" : "No") << std::endl;

            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.presentationFamily = i;
                std::cout << "Presentation family set to " << i << std::endl;
            }

            std::cout << "Queue Family Indices to be returned - Graphics: " << indices.graphicsFamily
              << ", Presentation: " << indices.presentationFamily << std::endl;

            if (indices.isValid()) {
                std::cout << "Valid queue families found: Graphics = " << indices.graphicsFamily
                        << ", Presentation = " << indices.presentationFamily << std::endl;
                storedIndices = indices; // Store the valid indices
                break;
            }

            i++;
        }

        return indices;
    }
}