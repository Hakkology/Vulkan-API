#include "pipelineShaderModule.h"

PipelineShaderModule::PipelineShaderModule(VkDevice device, VkShaderStageFlagBits stage, const std::string& shaderPath)
: device(device), shaderStage(stage), shaderPath(shaderPath) {
    module = createShaderModule(shaderPath);  // Automatically create the shader module
}

PipelineShaderModule::~PipelineShaderModule() {
    if (module != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device, module, nullptr);
        module = VK_NULL_HANDLE;
    }
}

VkShaderModule PipelineShaderModule::createShaderModule(const std::string& filename) {
    auto shaderCode = readShaderFile(filename);
    if (shaderCode.empty()) {
        throw std::runtime_error("Shader file is empty or not read correctly.");
    }
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module.");
    }
    return shaderModule;
}

VkPipelineShaderStageCreateInfo PipelineShaderModule::createShaderStageInfo() {
    
    shaderStageInfo = {};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = shaderStage;                     // Specify the pipeline stage the shader belongs to.
    shaderStageInfo.module = module;                         // Specify the shader module to use.
    shaderStageInfo.pName = "main";                          // Entry point of the shader; "main" by default.

    return shaderStageInfo;
}

std::vector<char> PipelineShaderModule::readShaderFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file.");
    }
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}
