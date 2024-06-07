#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "vulkanShaderPaths.h"

class GraphicsPipeline {
public:
    GraphicsPipeline(VkDevice device, VkRenderPass& renderPass, VkExtent2D& swapChainExtent);
    ~GraphicsPipeline();

    void createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
    void cleanup();
    
    VkPipeline getGraphicsPipeline() const {return graphicsPipeline;}

private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkDevice device;
    VkExtent2D swapChainExtent;
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineColorBlendAttachmentState colourState;
    VkRenderPass& renderPass;

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
