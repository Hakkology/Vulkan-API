#pragma once

#include <vulkan/vulkan.h>

class PipelineVertexInputState {
public:
    PipelineVertexInputState();
    ~PipelineVertexInputState();

    const VkPipelineVertexInputStateCreateInfo* getVertexInputState() const;

private:
    VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
    void createVertexInputState();
};
