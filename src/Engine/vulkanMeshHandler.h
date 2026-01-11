#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string.h>
#include <vector>

#include "vulkanUtils.h"

class Mesh {

public:
  Mesh();
  Mesh(VkPhysicalDevice physicalDevice, VkDevice device,
       std::vector<Vertex> *vertices);
  ~Mesh();

  void destroyVertexBuffer();

  inline VkBuffer getVertexBuffer() { return vertexBuffer; }
  inline int getVertexCount() { return vertexCount; }

  void setColor(const glm::vec4 &newColor) { color = newColor; }
  glm::vec4 getColor() const { return color; }

  void setTexture(VkImageView imageView, VkSampler sampler) {
    textureImageView = imageView;
    textureSampler = sampler;
  }
  bool hasTexture() const { return textureImageView != VK_NULL_HANDLE; }
  VkImageView getTextureImageView() const { return textureImageView; }
  VkSampler getTextureSampler() const { return textureSampler; }

private:
  int vertexCount;

  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer vertexBuffer;
  glm::vec4 color = glm::vec4(1.0f); // Default white

  VkImageView textureImageView = VK_NULL_HANDLE;
  VkSampler textureSampler = VK_NULL_HANDLE;
  VkDescriptorSet textureDescriptorSet = VK_NULL_HANDLE;

public:
  void setTextureDescriptorSet(VkDescriptorSet descriptorSet) {
    textureDescriptorSet = descriptorSet;
  }
  VkDescriptorSet getTextureDescriptorSet() const {
    return textureDescriptorSet;
  }

private:
  void createVertexBuffer(std::vector<Vertex> *vertices);
  uint32_t findMemoryTypeIndex(uint32_t allowedTypes,
                               VkMemoryPropertyFlags flags);
};
