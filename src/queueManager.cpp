#include "queueManager.h"

QueueManager::QueueManager() : graphicsQueue(VK_NULL_HANDLE) {
    // Constructor implementation, if needed.
}

void QueueManager::init(VkDevice logicalDevice, VkPhysicalDevice physicalDevice) {
    // Initialize queues
    // This should be called after creating the logical device
    QueueFamilyIndices indices = VulkanUtils::findQueueFamiliesForDevice(physicalDevice);
    createQueues(logicalDevice, indices);
}

VkQueue QueueManager::getGraphicsQueue() const {
    return graphicsQueue;
}

void QueueManager::createQueues(VkDevice logicalDevice, QueueFamilyIndices indices) {
    // Assuming there's only one graphics queue to be initialized
    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, indices.presentationFamily, 0, &presentationQueue);
}



