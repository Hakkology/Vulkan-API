#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "vulkanShaderPaths.h"

class PipelineShaderModule {
public:
    PipelineShaderModule(VkDevice device, VkShaderStageFlagBits stage, const std::string& shaderPath);
    ~PipelineShaderModule();

    VkPipelineShaderStageCreateInfo createShaderStageInfo();

private:
    VkDevice device;
    VkShaderModule module = VK_NULL_HANDLE;

    std::string shaderPath;
    VkShaderStageFlagBits shaderStage;
    VkPipelineShaderStageCreateInfo shaderStageInfo;

    std::vector<char> readShaderFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::string& filename);
};
