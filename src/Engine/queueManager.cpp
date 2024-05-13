#include "queueManager.h"

QueueManager::QueueManager() : graphicsQueue(VK_NULL_HANDLE), presentationQueue(VK_NULL_HANDLE) {
    // Constructor implementation, if needed.
}

bool QueueManager::init(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    // Initialize queues
    // This should be called after creating the logical device
    QueueFamilyIndices indices = VulkanUtils::findQueueFamiliesForSurface(physicalDevice, surface);
    if (!createQueues(logicalDevice, indices)) {
        std::cerr << "Failed to initialize queues." << std::endl;
        return false; // Return false if queue creation fails
    }
    return true; // Return true if queues are initialized successfully
    std::cout << "Queue Manager created successfully." << std::endl;
}

VkQueue QueueManager::getGraphicsQueue() const {
    return graphicsQueue;
}

bool QueueManager::createQueues(VkDevice logicalDevice, QueueFamilyIndices indices) {
    // Assuming there's only one graphics queue to be initialized
    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
    if (graphicsQueue == VK_NULL_HANDLE) {
        std::cerr << "Failed to get graphics queue." << std::endl;
        return false;
    }
    
    vkGetDeviceQueue(logicalDevice, indices.presentationFamily, 0, &presentationQueue);
    if (presentationQueue == VK_NULL_HANDLE) {
        std::cerr << "Failed to get presentation queue." << std::endl;
        return false;
    }

    std::cout << "Queues created successfully." << std::endl;
    return true; // Return true if queues are created successfully
}
