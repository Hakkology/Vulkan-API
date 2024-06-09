#include "pipelineDepthStencilState.h"

PipelineDepthStencilState::PipelineDepthStencilState(VkDevice device) : device(device) {
    createDepthStencilState();
}

PipelineDepthStencilState::~PipelineDepthStencilState() {
    // Herhangi bir Vulkan kaynağı temizleme işlemi gerekmiyor
}

void PipelineDepthStencilState::createDepthStencilState() {
    depthStencilStateInfo = {};
    // Depth stencil testing.
    // TODO: Set up depth stencil testing.
}

const VkPipelineDepthStencilStateCreateInfo* PipelineDepthStencilState::getDepthStencilStateInfo() const {
    return &depthStencilStateInfo;
}