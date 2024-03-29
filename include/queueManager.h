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
    void init(VkDevice logicalDevice, VkPhysicalDevice physicalDevice);
    VkQueue getGraphicsQueue() const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

private:
    VkQueue graphicsQueue;
    void createQueues(VkDevice logicalDevice, QueueFamilyIndices indices);

};
