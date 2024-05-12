// DeviceManager.h
#pragma once

#include <set>
#include "swapChainManager.h"
#include "queueManager.h"
#include "vulkanUtils.h"

class DeviceManager {
public:
    DeviceManager();
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, GLFWwindow* window);
    void createLogicalDevice(VkSurfaceKHR surface);
    void clearLogicalDevice() { logicalDevice = VK_NULL_HANDLE; }
    VkPhysicalDevice getPhysicalDevice() const;
    VkDevice getLogicalDevice() const;

private:
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    bool checkDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, GLFWwindow* window);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    // QueueFamilyIndices findQueueFamiliesForDevice(VkPhysicalDevice device);
    // QueueFamilyIndices findQueueFamiliesForSurface(VkPhysicalDevice device, VkSurfaceKHR surface);
};
