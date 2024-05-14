#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <vulkanUtils.h>
#include <swapChainManager.h>
#include "vulkanRenderpass.h"

class FrameManager {
public:
    FrameManager(VkDevice device, SwapChainManager& swapChainManager, VkRenderPass renderpass);
    ~FrameManager();

    void createFrameBuffers();
    void cleanup();

    std::vector<VkFramebuffer> getSwapchainFramebuffers() const { return swapchainFrameBuffers; }

private:
    VkDevice device;
    SwapChainManager& swapchainManager;
    VkRenderPass renderpass;

    std::vector<VkFramebuffer> swapchainFrameBuffers;
    std::vector<SwapchainImage> swapchainImages;

    // std::vector<VkSemaphore> imageAvailableSemaphores;
    // std::vector<VkSemaphore> renderFinishedSemaphores;
    // std::vector<VkFence> inFlightFences;

    // void createSemaphores();
    // void createFences();
};
