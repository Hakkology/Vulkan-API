#include "pipelineDynamicState.h"

PipelineDynamicState::PipelineDynamicState() {
    createDynamicState();
}

PipelineDynamicState::~PipelineDynamicState() {
    // Cleanup logic if needed
}

void PipelineDynamicState::createDynamicState() {
    // Dynamic State
    // Dynamic states to enable:
    // std::vector<VkDynamicState> dynamicStateEnables;
    // dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);                               // Dynamic viewport: can resize in command buffer with vkCmdSetViewport(commandbuffer, 0, 1, &viewport)
    // dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);                                // Dynamic scissor: can resize in command buffer with vkCmdSetScissor(commandbuffer, 0 ,1, &scissor)
    // // Do not destroy the pipeline but just pass the updated size to a new image on swapchain.

    dynamicStateCreateInfo = {};
    // dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    // dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();
}

const VkPipelineDynamicStateCreateInfo* PipelineDynamicState::getDynamicStateInfo() const {
    return &dynamicStateCreateInfo;
}
