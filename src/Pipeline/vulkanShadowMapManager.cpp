#include "vulkanShadowMapManager.h"
#include <iostream>
#include <stdexcept>

VulkanShadowMapManager::VulkanShadowMapManager(VkDevice device,
                                               VkPhysicalDevice physicalDevice)
    : device(device), physicalDevice(physicalDevice),
      shadowImage(VK_NULL_HANDLE), shadowImageMemory(VK_NULL_HANDLE),
      shadowImageView(VK_NULL_HANDLE), shadowSampler(VK_NULL_HANDLE),
      shadowRenderPass(VK_NULL_HANDLE), shadowFramebuffer(VK_NULL_HANDLE) {}

VulkanShadowMapManager::~VulkanShadowMapManager() { cleanup(); }

void VulkanShadowMapManager::cleanup() {
  if (shadowFramebuffer != VK_NULL_HANDLE)
    vkDestroyFramebuffer(device, shadowFramebuffer, nullptr);
  if (shadowRenderPass != VK_NULL_HANDLE)
    vkDestroyRenderPass(device, shadowRenderPass, nullptr);
  if (shadowSampler != VK_NULL_HANDLE)
    vkDestroySampler(device, shadowSampler, nullptr);
  if (shadowImageView != VK_NULL_HANDLE)
    vkDestroyImageView(device, shadowImageView, nullptr);
  if (shadowImage != VK_NULL_HANDLE)
    vkDestroyImage(device, shadowImage, nullptr);
  if (shadowImageMemory != VK_NULL_HANDLE)
    vkFreeMemory(device, shadowImageMemory, nullptr);

  shadowFramebuffer = VK_NULL_HANDLE;
  shadowRenderPass = VK_NULL_HANDLE;
  shadowSampler = VK_NULL_HANDLE;
  shadowImageView = VK_NULL_HANDLE;
  shadowImage = VK_NULL_HANDLE;
  shadowImageMemory = VK_NULL_HANDLE;
}

void VulkanShadowMapManager::createResources(uint32_t width, uint32_t height) {
  shadowWidth = width;
  shadowHeight = height;

  createImage(
      shadowWidth, shadowHeight, shadowFormat, VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, shadowImage, shadowImageMemory);

  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = shadowImage;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = shadowFormat;
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  if (vkCreateImageView(device, &viewInfo, nullptr, &shadowImageView) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create shadow image view!");
  }

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
  samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

  if (vkCreateSampler(device, &samplerInfo, nullptr, &shadowSampler) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create shadow sampler!");
  }
}

void VulkanShadowMapManager::createRenderPass() {
  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = shadowFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 0;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 0;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;

  std::array<VkSubpassDependency, 2> dependencies;
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

  dependencies[1].srcSubpass = 0;
  dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
  dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &depthAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
  renderPassInfo.pDependencies = dependencies.data();

  if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &shadowRenderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create shadow render pass!");
  }
}

void VulkanShadowMapManager::createFramebuffer() {
  VkFramebufferCreateInfo framebufferInfo{};
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = shadowRenderPass;
  framebufferInfo.attachmentCount = 1;
  framebufferInfo.pAttachments = &shadowImageView;
  framebufferInfo.width = shadowWidth;
  framebufferInfo.height = shadowHeight;
  framebufferInfo.layers = 1;

  if (vkCreateFramebuffer(device, &framebufferInfo, nullptr,
                          &shadowFramebuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shadow framebuffer!");
  }
}

glm::mat4
VulkanShadowMapManager::getLightSpaceMatrix(const glm::vec3 &lightDir) {
  glm::mat4 lightProjection =
      glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);
  glm::vec3 lightPos = -glm::normalize(lightDir) * 20.0f;
  glm::mat4 lightView =
      glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  return lightProjection * lightView;
}

void VulkanShadowMapManager::createImage(uint32_t width, uint32_t height,
                                         VkFormat format, VkImageTiling tiling,
                                         VkImageUsageFlags usage,
                                         VkMemoryPropertyFlags properties,
                                         VkImage &image,
                                         VkDeviceMemory &imageMemory) {
  VkImageCreateInfo imageInfo{};
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

  if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
    throw std::runtime_error("failed to create image!");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(device, image, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate image memory!");
  }

  vkBindImageMemory(device, image, imageMemory, 0);
}

uint32_t
VulkanShadowMapManager::findMemoryType(uint32_t typeFilter,
                                       VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }
  throw std::runtime_error("failed to find suitable memory type!");
}
