#include "pipelineColorBlendState.h"

PipelineColorBlendState::PipelineColorBlendState(VkDevice device) : device(device) {
    createColorBlendState();
}

PipelineColorBlendState::~PipelineColorBlendState() {

}

void PipelineColorBlendState::createColorBlendState() {
    // Blending
    // Blending decides how to blend a new colour being written to a fragment with the old value.

    // Blend attachment State (how blending is handled)
    colourState = {};                                       // Colours to apply blending to.
    colourState.blendEnable = VK_TRUE;                                                          // Enable blending

    // Blending uses equation: (srcCOlorBlendFactor * new colour) colorBlendOp (dstColorBlendFactor * old colour)
    colourState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colourState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colourState.colorBlendOp = VK_BLEND_OP_ADD;
    // Summarized: (VK_BLEND_FACTOR_SRC_ALPHA * new colour) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * old colour)
    //              (new colour alpha * new colour) + ((1- new colour alpha) * old colour)
    colourState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colourState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colourState.alphaBlendOp = VK_BLEND_OP_ADD;
    // Summarized: (1* newalpha) + (0* oldalpha)
    colourState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendCreateInfo = {};
    colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendCreateInfo.logicOpEnable = VK_FALSE;                                          // Alternative to calculations is to use logical operations.
    colorBlendCreateInfo.attachmentCount = 1;
    colorBlendCreateInfo.pAttachments = &colourState;
}

const VkPipelineColorBlendStateCreateInfo* PipelineColorBlendState::getColorBlendStateInfo() const {
    return &colorBlendCreateInfo;
}
