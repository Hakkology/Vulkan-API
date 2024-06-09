#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <string.h>

class PipelineMultisampleState {
public:
    PipelineMultisampleState();
    ~PipelineMultisampleState();

    const VkPipelineMultisampleStateCreateInfo* getMultisampleState() const;
    void createMultisampleState();

private:
    VkPipelineMultisampleStateCreateInfo multisampleState;
};
