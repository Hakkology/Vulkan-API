#pragma once
#include "vulkanUtils.h"
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

class PipelineLayout {
public:
  PipelineLayout(VkDevice device,
                 const std::vector<VkDescriptorSetLayout> &layouts);
  ~PipelineLayout();

  const VkPipelineLayoutCreateInfo *getLayoutInfo() const;
  VkPipelineLayout getPipelineLayout() const;
  bool createPipelineLayout();

private:
  VkDevice device;
  VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
  VkPipelineLayoutCreateInfo layoutInfo;
  VkPushConstantRange pushConstantRange;

  void setupLayoutInfo(const std::vector<VkDescriptorSetLayout> &layouts);
};