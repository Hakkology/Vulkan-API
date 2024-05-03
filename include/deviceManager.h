// DeviceManager.h
#pragma once

#include <set>
#include "queueManager.h"
#include "vulkanUtils.h"

class DeviceManager {
public:
    DeviceManager();
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    void createLogicalDevice(VkSurfaceKHR surface);
    void clearLogicalDevice() { logicalDevice = VK_NULL_HANDLE; }
    VkPhysicalDevice getPhysicalDevice() const;
    VkDevice getLogicalDevice() const;

private:
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    bool checkDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    // QueueFamilyIndices findQueueFamiliesForDevice(VkPhysicalDevice device);
    // QueueFamilyIndices findQueueFamiliesForSurface(VkPhysicalDevice device, VkSurfaceKHR surface);
};
