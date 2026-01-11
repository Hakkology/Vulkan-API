#include "pipelineDepthStencilState.h"

PipelineDepthStencilState::PipelineDepthStencilState(VkDevice device)
    : device(device) {
  createDepthStencilState();
}

PipelineDepthStencilState::~PipelineDepthStencilState() {
  // Herhangi bir Vulkan kaynağı temizleme işlemi gerekmiyor
}

void PipelineDepthStencilState::createDepthStencilState() {
  depthStencilStateInfo = {};
  depthStencilStateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencilStateInfo.depthTestEnable = VK_TRUE;
  depthStencilStateInfo.depthWriteEnable = VK_TRUE;
  depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
  depthStencilStateInfo.minDepthBounds = 0.0f; // Optional
  depthStencilStateInfo.maxDepthBounds = 1.0f; // Optional
  depthStencilStateInfo.stencilTestEnable = VK_FALSE;
  depthStencilStateInfo.front = {}; // Optional
  depthStencilStateInfo.back = {};  // Optional
}

const VkPipelineDepthStencilStateCreateInfo *
PipelineDepthStencilState::getDepthStencilStateInfo() const {
  return &depthStencilStateInfo;
}