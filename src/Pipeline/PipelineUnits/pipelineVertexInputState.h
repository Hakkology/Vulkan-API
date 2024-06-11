#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

#include "vulkanUtils.h"



class PipelineVertexInputState {
public:
    PipelineVertexInputState(VkDevice device);
    ~PipelineVertexInputState();

    const VkPipelineVertexInputStateCreateInfo* getVertexInputState() const;

private:
    VkDevice device;
    VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
    void createVertexInputState();
};
