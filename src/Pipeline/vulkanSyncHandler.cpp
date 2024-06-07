#include "vulkanSyncHandler.h"

SynchronizationHandler::SynchronizationHandler(VkDevice device, size_t frameCount)
    : device(device), frameCount(frameCount) {
    createSynchronizationObjects();  // Optionally, create objects on construction
}

SynchronizationHandler::~SynchronizationHandler() {
    cleanup();  // Clean up resources in the destructor.
}

void SynchronizationHandler::createSynchronizationObjects() {
    imageAvailableSemaphores.resize(frameCount);
    renderFinishedSemaphores.resize(frameCount);
    inFlightFences.resize(frameCount);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Initialize as signaled

    for (size_t i = 0; i < frameCount; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            cleanup();  // Cleanup in case of failure during initialization
            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
    }
}

void SynchronizationHandler::cleanup() {
    for (auto& semaphore : imageAvailableSemaphores) {
        if (semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, semaphore, nullptr);
            semaphore = VK_NULL_HANDLE;
        }
    }
    for (auto& semaphore : renderFinishedSemaphores) {
        if (semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, semaphore, nullptr);
            semaphore = VK_NULL_HANDLE;
        }
    }
    for (auto& fence : inFlightFences) {
        if (fence != VK_NULL_HANDLE) {
            vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX); // Wait for fence before destruction
            vkDestroyFence(device, fence, nullptr);
            fence = VK_NULL_HANDLE;
        }
    }
}
