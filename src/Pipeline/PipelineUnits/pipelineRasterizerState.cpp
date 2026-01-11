#include "pipelineRasterizerState.h"

PipelineRasterizerState::PipelineRasterizerState(VkCullModeFlags cullMode,
                                                 VkFrontFace frontFace,
                                                 VkBool32 depthBiasEnable)
    : cullMode(cullMode), frontFace(frontFace),
      depthBiasEnable(depthBiasEnable) {
  createRasterizationState();
}

PipelineRasterizerState::~PipelineRasterizerState() {}

const VkPipelineRasterizationStateCreateInfo *
PipelineRasterizerState::getRasterizationStateInfo() const {
  return &rasterizationState;
}

void PipelineRasterizerState::createRasterizationState() {
  rasterizationState = {};
  // Rasterizer
  rasterizationState.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizationState.depthClampEnable =
      VK_FALSE; // Change if fragments beyond near/far planes are clipped or
                // clamped.
  // Device features in logical device must be enabled if this is toggled on.
  rasterizationState.rasterizerDiscardEnable =
      VK_FALSE; // Whether to discard data and skip rasterizer. NEver creaters
                // fragments, suitable for pipelines without graphics.
  rasterizationState.polygonMode =
      VK_POLYGON_MODE_FILL; // How to handle filling points between vertices.
  // GPU features are needed for some.
  rasterizationState.lineWidth = 1.0f; // How thick lines should be when drawn.
  rasterizationState.cullMode = cullMode;
  rasterizationState.frontFace = frontFace;
  rasterizationState.depthBiasEnable = depthBiasEnable;
  if (depthBiasEnable) {
    rasterizationState.depthBiasConstantFactor = 1.25f;
    rasterizationState.depthBiasSlopeFactor = 1.75f;
    rasterizationState.depthBiasClamp = 0.0f;
  }
}
