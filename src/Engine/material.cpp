#include "material.h"

Material::Material() : type(MaterialType::COLORED) {}

Material::Material(MaterialType type) : type(type) {}

Material::~Material() {
  // Note: Vulkan resources (imageView, sampler, descriptorSet) are not owned by
  // Material They should be destroyed by their respective managers
}

void Material::setTexture(VkImageView imageView, VkSampler sampler) {
  textureImageView = imageView;
  textureSampler = sampler;
  type = MaterialType::TEXTURED;
}

void Material::setDescriptorSet(VkDescriptorSet ds) { descriptorSet = ds; }
