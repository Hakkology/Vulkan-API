#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanShadowMapManager {
public:
  VulkanShadowMapManager(VkDevice device, VkPhysicalDevice physicalDevice);
  ~VulkanShadowMapManager();

  void createResources(uint32_t width, uint32_t height);
  void createRenderPass();
  void createFramebuffer();
  void cleanup();

  VkImageView getShadowImageView() const { return shadowImageView; }
  VkSampler getShadowSampler() const { return shadowSampler; }
  VkRenderPass getRenderPass() const { return shadowRenderPass; }
  VkFramebuffer getFramebuffer() const { return shadowFramebuffer; }
  VkExtent2D getExtent() const { return {shadowWidth, shadowHeight}; }
  uint32_t getShadowWidth() const { return shadowWidth; }
  uint32_t getShadowHeight() const { return shadowHeight; }

  glm::mat4 getLightSpaceMatrix(const glm::vec3 &lightDir);

private:
  VkDevice device;
  VkPhysicalDevice physicalDevice;

  VkImage shadowImage;
  VkDeviceMemory shadowImageMemory;
  VkImageView shadowImageView;
  VkSampler shadowSampler;

  VkRenderPass shadowRenderPass;
  VkFramebuffer shadowFramebuffer;

  uint32_t shadowWidth = 2048;
  uint32_t shadowHeight = 2048;

  VkFormat shadowFormat = VK_FORMAT_D32_SFLOAT;

  void createImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkImage &image,
                   VkDeviceMemory &imageMemory);
  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
};
