#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

struct Vertex{
    glm::vec3 pos; // vertex position (x, y, z)
};

class PipelineVertexInputState {
public:
    PipelineVertexInputState();
    ~PipelineVertexInputState();

    const VkPipelineVertexInputStateCreateInfo* getVertexInputState() const;

private:
    VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
    void createVertexInputState();
};
