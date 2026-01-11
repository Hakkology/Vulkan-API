#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <string.h>
#include <vector>

#include "material.h"
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

  // Material
  void setMaterial(std::shared_ptr<Material> mat) { material = mat; }
  std::shared_ptr<Material> getMaterial() const { return material; }
  bool hasMaterial() const { return material != nullptr; }

  // Convenience methods that delegate to Material
  bool hasTexture() const { return material && material->hasTexture(); }
  glm::vec4 getColor() const {
    return material ? material->getColor() : glm::vec4(1.0f);
  }
  VkDescriptorSet getTextureDescriptorSet() const {
    return material ? material->getDescriptorSet() : VK_NULL_HANDLE;
  }

  void setModelMatrix(const glm::mat4 &matrix) { modelMatrix = matrix; }
  glm::mat4 getModelMatrix() const { return modelMatrix; }

private:
  glm::mat4 modelMatrix{1.0f};
  int vertexCount;

  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer vertexBuffer;

  std::shared_ptr<Material> material;

  void createVertexBuffer(std::vector<Vertex> *vertices);
  uint32_t findMemoryTypeIndex(uint32_t allowedTypes,
                               VkMemoryPropertyFlags flags);
};
