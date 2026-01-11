#pragma once

#include "../Engine/swapChainManager.h"
#include "../Engine/vulkanUtils.h"
#include "vulkanRenderpass.h"
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

class FrameManager {
public:
  FrameManager(VkDevice device, SwapChainManager &swapChainManager,
               VkRenderPass renderpass);
  ~FrameManager();

  void createFrameBuffers(const std::vector<VkImageView> &depthImageViews);
  void cleanup();

  std::vector<VkFramebuffer> getSwapchainFramebuffers() const {
    return swapchainFrameBuffers;
  }

private:
  VkDevice device;
  SwapChainManager &swapchainManager;
  VkRenderPass renderpass;

  std::vector<VkFramebuffer> swapchainFrameBuffers;
  std::vector<SwapchainImage> swapchainImages;

  // std::vector<VkSemaphore> imageAvailableSemaphores;
  // std::vector<VkSemaphore> renderFinishedSemaphores;
  // std::vector<VkFence> inFlightFences;

  // void createSemaphores();
  // void createFences();
};
