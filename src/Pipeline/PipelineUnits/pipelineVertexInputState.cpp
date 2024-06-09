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

    // How the data for a single vertex (including info such as position, colour, texture coords, normals etc.)

    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;                                                        // Can bind multiple streams of data, this defines which one
    bindingDescription.stride = sizeof(Vertex);                                            // Size of a single vertex object
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;                            // How to move between data after each vertex.
                                                                                           // VK_VERTEX_INPUT_RATE_INDEX : Move on to the next vertex
                                                                                           // VK_VERTEX_INPUT_RATE_INSTANCE : Move to a vertex for the next
    
    // How the data for an attribute is defined within a vertex.
    std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions;

    // Position attributes.
    attributeDescriptions[0].binding = 0;


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
