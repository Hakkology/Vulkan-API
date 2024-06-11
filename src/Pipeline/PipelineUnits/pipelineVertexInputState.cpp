#include "pipelineVertexInputState.h"

PipelineVertexInputState::PipelineVertexInputState(VkDevice device) : device(device) {
    createVertexInputState();
}

PipelineVertexInputState::~PipelineVertexInputState() {
    // Destructor logic if needed
}

// Vertex Input, we should put in vertex descriptions when resources are created.
void PipelineVertexInputState::createVertexInputState()
{
    // Vertex Input 

    // Binding description.
    // How the data for a single vertex (including info such as position, colour, texture coords, normals etc.)

    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;                                                        // Can bind multiple streams of data, this defines which one
    bindingDescription.stride = sizeof(Vertex);                                            // Size of a single vertex object
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;                            // How to move between data after each vertex.
                                                                                           // VK_VERTEX_INPUT_RATE_INDEX : Move on to the next vertex
                                                                                           // VK_VERTEX_INPUT_RATE_INSTANCE : Move to a vertex for the next
    // How the data for an attribute is defined within a vertex.
    // binding attribute is hidden for pos variable in shader.
    std::array<VkVertexInputAttributeDescription, 1> attributeDescription;

    // Position attribute description.
    attributeDescription[0].binding = 0;                                                  // which binding the data is at
    attributeDescription[0].location = 0;                                                 // location in shader where data will be read from.
    attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;                          // format the data will take (helps define size of data)
    attributeDescription[0].offset = offsetof(Vertex, pos);                               // where this attribute is defined in the data for a single vertex
    // go to vertex data, what is the offset of the vertex data ? Basically moves next data by size of the first data.

    // Color Attribute if available.

    vertexInputStateInfo = {};
    vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateInfo.vertexBindingDescriptionCount = 1;
    vertexInputStateInfo.pVertexBindingDescriptions = &bindingDescription;                  // list of vertex binding descriptions (data spacing & stride information)
    vertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());                          
    vertexInputStateInfo.pVertexAttributeDescriptions = attributeDescription.data();        // list of vertex attribute descriptions (data format and where to bind to/from)
}

const VkPipelineVertexInputStateCreateInfo* PipelineVertexInputState::getVertexInputState() const {
    return &vertexInputStateInfo;
}
