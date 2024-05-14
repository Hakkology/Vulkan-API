#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <iostream>

class SynchronizationHandler {
public:
    SynchronizationHandler(VkDevice device, int maxFramesInFlight);
    ~SynchronizationHandler();

    void createSynchronization();
    void cleanup();

    std::vector<VkSemaphore> imageAvailable;
    std::vector<VkSemaphore> renderFinished;
    std::vector<VkFence> inFlightFences;

private:
    VkDevice device;
    int maxFramesInFlight;
};