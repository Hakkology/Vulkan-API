#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "vulkanUtils.h"

class PipelineVertexInputState {
public:
  PipelineVertexInputState(VkDevice device);
  ~PipelineVertexInputState();

  const VkPipelineVertexInputStateCreateInfo *getVertexInputState() const;

private:
  VkDevice device;
  VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;

  VkVertexInputBindingDescription bindingDescription;
  std::array<VkVertexInputAttributeDescription, 2> attributeDescription;

  void createVertexInputState();
};
