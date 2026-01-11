#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

enum class MaterialType { COLORED, TEXTURED, COLORED_MOVING };

class Material {
public:
  Material();
  Material(MaterialType type);
  ~Material();

  // Type
  MaterialType getType() const { return type; }
  void setType(MaterialType newType) { type = newType; }

  // Color
  void setColor(const glm::vec4 &newColor) { color = newColor; }
  glm::vec4 getColor() const { return color; }

  // Texture
  void setTexture(VkImageView imageView, VkSampler sampler);
  bool hasTexture() const { return textureImageView != VK_NULL_HANDLE; }
  VkImageView getTextureImageView() const { return textureImageView; }
  VkSampler getTextureSampler() const { return textureSampler; }

  // Descriptor Set
  void setDescriptorSet(VkDescriptorSet descriptorSet);
  VkDescriptorSet getDescriptorSet() const { return descriptorSet; }

private:
  MaterialType type = MaterialType::COLORED;
  glm::vec4 color = glm::vec4(1.0f); // Default white

  VkImageView textureImageView = VK_NULL_HANDLE;
  VkSampler textureSampler = VK_NULL_HANDLE;
  VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};
