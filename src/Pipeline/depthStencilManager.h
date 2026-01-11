#pragma once

#include "../Engine/swapChainManager.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

class DepthStencilManager {
public:
  DepthStencilManager(VkPhysicalDevice physicalDevice, VkDevice logicalDevice);
  ~DepthStencilManager();

  void createDepthResources(VkExtent2D swapChainExtent,
                            SwapChainManager *swapChainManager);
  void cleanup();

  std::vector<VkImageView> getDepthImageViews() const {
    return depthImageViews;
  }
  VkFormat getDepthFormat() const { return depthFormat; }

private:
  VkPhysicalDevice physicalDevice;
  VkDevice logicalDevice;

  std::vector<VkImage> depthImages;
  std::vector<VkDeviceMemory> depthImageMemorys;
  std::vector<VkImageView> depthImageViews;
  VkFormat depthFormat;

  VkFormat findDepthFormat();
  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  void createImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkImage &image,
                   VkDeviceMemory &imageMemory);
};
