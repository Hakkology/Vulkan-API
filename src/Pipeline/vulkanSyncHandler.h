#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

class SynchronizationHandler {
public:
    SynchronizationHandler(VkDevice device, size_t frameCount);
    ~SynchronizationHandler();

    void createSynchronizationObjects();
    void cleanup();

    // Use vectors to handle multiple frames
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    size_t frameCount;

private:
    VkDevice device;

};
