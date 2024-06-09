#pragma once

#include <vulkan/vulkan.h>
#include <iostream>

class PipelineViewportState {
public:
    PipelineViewportState(VkDevice device, VkExtent2D swapChainExtent);
    ~PipelineViewportState();

    VkPipelineViewportStateCreateInfo createViewportState();
    const VkPipelineViewportStateCreateInfo* getViewportStateCreateInfo() const;
    

private:
    VkDevice device;
    VkExtent2D swapChainExtent;
    VkViewport viewport;
    VkRect2D scissor;

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
};

