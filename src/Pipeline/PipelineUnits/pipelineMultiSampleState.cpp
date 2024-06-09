#include "pipelineMultiSampleState.h"


PipelineMultisampleState::PipelineMultisampleState() {
    createMultisampleState();
}

PipelineMultisampleState::~PipelineMultisampleState() {}

const VkPipelineMultisampleStateCreateInfo* PipelineMultisampleState::getMultisampleState() const {
    return &multisampleState;
}

void PipelineMultisampleState::createMultisampleState()
{
    multisampleState = {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pNext = nullptr;
    multisampleState.sampleShadingEnable = VK_FALSE;  // Enable multi sampling shading or not.
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;  // Number of samples to use per fragment.
}


