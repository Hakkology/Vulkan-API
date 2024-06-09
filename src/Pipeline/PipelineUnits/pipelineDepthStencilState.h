#pragma once

#include <vulkan/vulkan.h>

class PipelineDepthStencilState {
public:
    PipelineDepthStencilState(VkDevice device);
    ~PipelineDepthStencilState();

    const VkPipelineDepthStencilStateCreateInfo* getDepthStencilStateInfo() const;

private:
    VkDevice device;
    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo;

    void createDepthStencilState();
};