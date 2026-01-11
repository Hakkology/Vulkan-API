#pragma once

#include <vulkan/vulkan.h>

class PipelineRasterizerState {
public:
  PipelineRasterizerState(VkCullModeFlags cullMode = VK_CULL_MODE_NONE,
                          VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE,
                          VkBool32 depthBiasEnable = VK_FALSE);
  ~PipelineRasterizerState();

  const VkPipelineRasterizationStateCreateInfo *
  getRasterizationStateInfo() const;
  void createRasterizationState();

private:
  VkPipelineRasterizationStateCreateInfo rasterizationState;
  VkCullModeFlags cullMode;
  VkFrontFace frontFace;
  VkBool32 depthBiasEnable;
};
