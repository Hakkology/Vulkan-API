#pragma once

#include "swapChainManager.h"
#include <vector>
#include <vulkan/vulkan.h>

class VulkanDepthStencilManager {
public:
  VulkanDepthStencilManager(VkPhysicalDevice physicalDevice,
                            VkDevice logicalDevice);
  ~VulkanDepthStencilManager();

  void createDepthResources(VkExtent2D swapChainExtent,
                            SwapChainManager *swapChainManager);
  void cleanup();

  VkFormat getDepthFormat() const { return depthFormat; }
  const std::vector<VkImageView> &getDepthImageViews() const {
    return depthImageViews;
  }

private:
  VkPhysicalDevice physicalDevice;
  VkDevice logicalDevice;

  VkFormat depthFormat;
  std::vector<VkImage> depthImages;
  std::vector<VkDeviceMemory> depthImageMemorys;
  std::vector<VkImageView> depthImageViews;

  VkFormat findDepthFormat();
  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);

  void createImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkImage &image,
                   VkDeviceMemory &imageMemory);
};
