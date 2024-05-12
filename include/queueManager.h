#pragma once

#define GLFW_INCLUDE_VULKAN

#include <stdexcept>
#include <vector>
#include <string.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "vulkanUtils.h"


class QueueManager {
public:
    QueueManager();
    bool init(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    VkQueue getGraphicsQueue() const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const;

private:
    VkQueue graphicsQueue;
    VkQueue presentationQueue;
    bool createQueues(VkDevice logicalDevice, QueueFamilyIndices indices);

};
