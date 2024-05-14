#include "vulkanSyncHandler.h"

SynchronizationHandler::SynchronizationHandler(VkDevice device, int maxFramesInFlight)
    : device(device), maxFramesInFlight(maxFramesInFlight) {}

SynchronizationHandler::~SynchronizationHandler() {
    cleanup();
}

void SynchronizationHandler::createSynchronization() {
    imageAvailable.resize(maxFramesInFlight);
    renderFinished.resize(maxFramesInFlight);
    inFlightFences.resize(maxFramesInFlight);

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < maxFramesInFlight; ++i) {
        if (vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailable[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderFinished[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceCreateInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
    }
}

void SynchronizationHandler::cleanup() {
    for (int i = 0; i < maxFramesInFlight; ++i) {
        if (renderFinished[i] != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, renderFinished[i], nullptr);
            renderFinished[i] = VK_NULL_HANDLE;
        }

        if (imageAvailable[i] != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, imageAvailable[i], nullptr);
            imageAvailable[i] = VK_NULL_HANDLE;
        }

        if (inFlightFences[i] != VK_NULL_HANDLE) {
            vkDestroyFence(device, inFlightFences[i], nullptr);
            inFlightFences[i] = VK_NULL_HANDLE;
        }
    }
}