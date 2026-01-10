#include "pipelineLayoutState.h"

PipelineLayout::PipelineLayout(VkDevice device) : device(device) {
  setupLayoutInfo();
  createPipelineLayout();
}

PipelineLayout::~PipelineLayout() {
  if (pipelineLayout != VK_NULL_HANDLE) {
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    pipelineLayout = VK_NULL_HANDLE;
    std::cout << "Pipeline layout destroyed." << std::endl;
  }
}

void PipelineLayout::setupLayoutInfo() {
  // Defines push constant range for local variables in shaders.
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(PushConstants);

  layoutInfo = {};
  layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layoutInfo.setLayoutCount = 0;
  layoutInfo.pSetLayouts = nullptr;
  layoutInfo.pushConstantRangeCount = 1;
  layoutInfo.pPushConstantRanges = &pushConstantRange;
}

bool PipelineLayout::createPipelineLayout() {
  VkResult result =
      vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout);
  if (result != VK_SUCCESS) {
    std::cerr << "Failed to create pipeline layout: " << result << std::endl;
    return false;
  }
  return true;
}

VkPipelineLayout PipelineLayout::getPipelineLayout() const {
  return pipelineLayout;
}

const VkPipelineLayoutCreateInfo *PipelineLayout::getLayoutInfo() const {
  return &layoutInfo;
}