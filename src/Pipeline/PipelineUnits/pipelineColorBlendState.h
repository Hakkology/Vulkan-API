#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class PipelineColorBlendState {
public:
    PipelineColorBlendState(VkDevice device);
    ~PipelineColorBlendState();

    const VkPipelineColorBlendStateCreateInfo* getColorBlendStateInfo() const;

private:
    VkDevice device;
    VkPipelineColorBlendAttachmentState colourState;
    VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo;

    void createColorBlendState();
};
