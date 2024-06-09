#include "pipelineVertexInputState.h"

PipelineVertexInputState::PipelineVertexInputState() {
    createVertexInputState();
}

PipelineVertexInputState::~PipelineVertexInputState() {
    // Destructor logic if needed
}

// Vertex Input, we should put in vertex descriptions when resources are created.
void PipelineVertexInputState::createVertexInputState()
{
    // Vertex Input 
    vertexInputStateInfo = {};
    vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateInfo.vertexBindingDescriptionCount = 0;
    vertexInputStateInfo.pVertexBindingDescriptions = nullptr;                             // list of vertex binding descriptions (data spacing & stride information)
    vertexInputStateInfo.vertexAttributeDescriptionCount = 0;                          
    vertexInputStateInfo.pVertexAttributeDescriptions = nullptr;                           // list of vertex attribute descriptions (data format and where to bind to/from)
}

const VkPipelineVertexInputStateCreateInfo* PipelineVertexInputState::getVertexInputState() const {
    return &vertexInputStateInfo;
}
