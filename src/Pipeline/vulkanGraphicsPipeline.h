#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "vulkanShaderPaths.h"

class GraphicsPipeline {
public:
    GraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent);
    ~GraphicsPipeline();

    void createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
    void cleanup();

private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkDevice device;
    VkRenderPass renderPass;
    VkExtent2D swapChainExtent;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    std::vector<char> readShaderFile(const std::string& filename);

    VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
    VkPipelineVertexInputStateCreateInfo createVertexInputState();
    VkPipelineInputAssemblyStateCreateInfo createInputAssemblyState();
    VkPipelineRasterizationStateCreateInfo createRasterizerState();
    VkPipelineMultisampleStateCreateInfo createMultisampleState();
    VkPipelineColorBlendStateCreateInfo createColorBlendState();
    VkPipelineLayoutCreateInfo createPipelineLayoutInfo();
    VkPipelineDepthStencilStateCreateInfo createDepthStencilState();
    VkPipelineViewportStateCreateInfo createViewportState();
    VkPipelineDynamicStateCreateInfo createDynamicState();
};