#include "pipelineVertexInputState.h"

PipelineVertexInputState::PipelineVertexInputState(VkDevice device)
    : device(device) {
  createVertexInputState();
}

PipelineVertexInputState::~PipelineVertexInputState() {
  // Destructor logic if needed
}

// Vertex Input, we should put in vertex descriptions when resources are
// created.
void PipelineVertexInputState::createVertexInputState() {
  // Vertex Input

  // How vertex data is structured (pos, normal, tex, etc.)

  bindingDescription = {};
  bindingDescription.binding =
      0; // Can bind multiple streams of data, this defines which one
  bindingDescription.stride = sizeof(Vertex); // Size of a single vertex object
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  // How the data for an attribute is defined within a vertex.
  // binding attribute is hidden for pos variable in shader.
  // attributeDescription member is used directly

  // Position attribute description.
  attributeDescription[0].binding = 0; // which binding the data is at
  attributeDescription[0].location =
      0; // location in shader where data will be read from.
  attributeDescription[0].format =
      VK_FORMAT_R32G32B32_SFLOAT; // format the data will take (helps define
                                  // size of data)
  attributeDescription[0].offset = offsetof(Vertex, pos);

  // Normal Attribute
  attributeDescription[1].binding = 0;
  attributeDescription[1].location = 1;
  attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescription[1].offset = offsetof(Vertex, normal);

  // Texture Attribute
  attributeDescription[2].binding = 0;
  attributeDescription[2].location = 2;
  attributeDescription[2].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescription[2].offset = offsetof(Vertex, tex);

  vertexInputStateInfo = {};
  vertexInputStateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputStateInfo.vertexBindingDescriptionCount = 1;
  vertexInputStateInfo.pVertexBindingDescriptions =
      &bindingDescription; // list of vertex binding descriptions (data spacing
                           // & stride information)
  vertexInputStateInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescription.size());
  vertexInputStateInfo.pVertexAttributeDescriptions =
      attributeDescription.data(); // list of vertex attribute descriptions
                                   // (data format and where to bind to/from)
}

const VkPipelineVertexInputStateCreateInfo *
PipelineVertexInputState::getVertexInputState() const {
  return &vertexInputStateInfo;
}
