#include "pipelineInputAssemblyState.h"

PipelineInputAssemblyState::PipelineInputAssemblyState() {
    createInputAssemblyState();
}

PipelineInputAssemblyState::~PipelineInputAssemblyState() {
    // Destructor logic if needed
}

void PipelineInputAssemblyState::createInputAssemblyState() {
    // Input assembly
    inputAssemblyStateInfo = {};
    inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;                           // Primitive type to assemble vertices as.
    inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;                                        // Allow overriding of "strip" topology to start new primitives.
}

const VkPipelineInputAssemblyStateCreateInfo* PipelineInputAssemblyState::getInputAssemblyStateInfo() const {
    return &inputAssemblyStateInfo;
}
