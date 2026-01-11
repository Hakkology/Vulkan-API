#include "vulkanDepthStencilManager.h"
#include <iostream>

VulkanDepthStencilManager::VulkanDepthStencilManager(
    VkPhysicalDevice physicalDevice, VkDevice logicalDevice)
    : physicalDevice(physicalDevice), logicalDevice(logicalDevice) {}

VulkanDepthStencilManager::~VulkanDepthStencilManager() { cleanup(); }

void VulkanDepthStencilManager::cleanup() {
  for (auto imageView : depthImageViews) {
    vkDestroyImageView(logicalDevice, imageView, nullptr);
  }
  depthImageViews.clear();

  for (auto image : depthImages) {
    vkDestroyImage(logicalDevice, image, nullptr);
  }
  depthImages.clear();

  for (auto memory : depthImageMemorys) {
    vkFreeMemory(logicalDevice, memory, nullptr);
  }
  depthImageMemorys.clear();
}

void VulkanDepthStencilManager::createDepthResources(
    VkExtent2D swapChainExtent, SwapChainManager *swapChainManager) {
  depthFormat = findDepthFormat();

  size_t imageCount = swapChainManager->getSwapChainImages().size();
  depthImages.resize(imageCount);
  depthImageMemorys.resize(imageCount);
  depthImageViews.resize(imageCount);

  for (size_t i = 0; i < imageCount; i++) {
    createImage(swapChainExtent.width, swapChainExtent.height, depthFormat,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImages[i],
                depthImageMemorys[i]);

    depthImageViews[i] = swapChainManager->createImageView(
        logicalDevice, depthImages[i], depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
  }

  std::cout << "Depth resources created (" << imageCount << ")." << std::endl;
}

VkFormat VulkanDepthStencilManager::findDepthFormat() {
  return findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkFormat VulkanDepthStencilManager::findSupportedFormat(
    const std::vector<VkFormat> &candidates, VkImageTiling tiling,
    VkFormatFeatureFlags features) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("Failed to find supported format!");
}

void VulkanDepthStencilManager::createImage(
    uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
    VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
    VkDeviceMemory &imageMemory) {
  VkImageCreateInfo imageInfo = {};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateImage(logicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image!");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(logicalDevice, image, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;

  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  uint32_t memoryTypeIndex = UINT32_MAX;
  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((memRequirements.memoryTypeBits & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      memoryTypeIndex = i;
      break;
    }
  }

  if (memoryTypeIndex == UINT32_MAX) {
    throw std::runtime_error("failed to find suitable memory type!");
  }

  allocInfo.memoryTypeIndex = memoryTypeIndex;

  if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &imageMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate image memory!");
  }

  vkBindImageMemory(logicalDevice, image, imageMemory, 0);
}
