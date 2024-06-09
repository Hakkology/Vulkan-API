#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class PipelineDynamicState {
public:
    PipelineDynamicState();
    ~PipelineDynamicState();

    const VkPipelineDynamicStateCreateInfo* getDynamicStateInfo() const;

private:
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
    std::vector<VkDynamicState> dynamicStates;

    void createDynamicState();
};
