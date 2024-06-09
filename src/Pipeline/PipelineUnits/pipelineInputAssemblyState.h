#pragma once

#include <vulkan/vulkan.h>

class PipelineInputAssemblyState {
public:
    PipelineInputAssemblyState();
    ~PipelineInputAssemblyState();

    const VkPipelineInputAssemblyStateCreateInfo* getInputAssemblyStateInfo() const;

private:
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;
    void createInputAssemblyState();
};
