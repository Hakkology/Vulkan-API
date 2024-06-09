#include "pipelineViewportState.h"

PipelineViewportState::PipelineViewportState(VkDevice device, VkExtent2D swapChainExtent)
: device(device), swapChainExtent(swapChainExtent) {
    createViewportState();
}

PipelineViewportState::~PipelineViewportState() {}

const VkPipelineViewportStateCreateInfo* PipelineViewportState::getViewportStateCreateInfo() const {
    return &viewportStateCreateInfo;
}

VkPipelineViewportStateCreateInfo PipelineViewportState::createViewportState() {
    // Viewport & Scissor

    // Create a viewport
    viewport.x = 0.0f;                                                                      // x Start coordinates
    viewport.y = 0.0f;                                                                      // y Start coordinate.
    viewport.width = static_cast<float> (swapChainExtent.width);                                         // viewport width
    viewport.height = static_cast<float> (swapChainExtent.height);                                       // viewport height
    viewport.minDepth = 0.0f;                                                               // min framebuffer depth
    viewport.maxDepth = 1.0f;                                                               // max framebuffer depth

    // Scissor configuration
    scissor.offset = {0,0};                                                                 // offset to use region from
    scissor.extent = swapChainExtent;                                                       // extent to describe region to use, starting at offset

    std::cout << "Scissor - Offset X: " << scissor.offset.x << ", Offset Y: " << scissor.offset.y << ", Width: " << scissor.extent.width << ", Height: " << scissor.extent.height << std::endl;

    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.pNext = nullptr;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;

    return viewportStateCreateInfo;
}
