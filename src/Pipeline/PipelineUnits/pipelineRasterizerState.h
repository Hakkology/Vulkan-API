#pragma once

#include <vulkan/vulkan.h>

class PipelineRasterizerState {
public:
    PipelineRasterizerState();
    ~PipelineRasterizerState();

    const VkPipelineRasterizationStateCreateInfo* getRasterizationStateInfo() const;
    void createRasterizationState();

private:
    VkPipelineRasterizationStateCreateInfo rasterizationState;
};
