// VulkanUtils.h
#pragma once

#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
    int graphicsFamily = -1;                            // Graphics Queue Family location.
    int presentationFamily = -1;                        // Presentation Queue Family location.

    bool isValid() const {
        return graphicsFamily >= 0 && presentationFamily >=0;
    }
};

struct SwapChainDetails{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;       // Surface Properties e.g. image size/extent.
    std::vector<VkSurfaceFormatKHR> formats;            // Surface Image format e.g. RGBA and size of each colour.
    std::vector<VkPresentModeKHR> presentationModes;    // How images should be presented to screen.
};

namespace VulkanUtils {
    // Finds queue families without considering presentation support
    QueueFamilyIndices findQueueFamiliesForDevice(VkPhysicalDevice device);

    // Finds queue families with presentation support considered
    QueueFamilyIndices findQueueFamiliesForSurface(VkPhysicalDevice device, VkSurfaceKHR surface);
}
