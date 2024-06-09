#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "pipelineShaderModule.h"
#include "pipelineVertexInputState.h"
#include "pipelineViewportState.h"
#include "pipelineRasterizerState.h"
#include "pipelineMultiSampleState.h"
#include "pipelineColorBlendState.h"
#include "pipelineDepthStencilState.h"
#include "pipelineLayoutState.h"

class GraphicsPipeline {
public:
    GraphicsPipeline(VkDevice device, VkRenderPass& renderPass, VkExtent2D& swapChainExtent);
    ~GraphicsPipeline();

    void createGraphicsPipeline();
    void cleanup();
    void resetPipelineUnits();
    
    VkPipeline getGraphicsPipeline() const {return graphicsPipeline;}

private:
    VkPipeline graphicsPipeline;
    //VkPipelineLayout pipelineLayout;
    VkDevice device;
    VkExtent2D swapChainExtent;
    // VkViewport viewport;
    // VkRect2D scissor;
    // VkPipelineColorBlendAttachmentState colourState;
    VkRenderPass& renderPass;

    // VkShaderModule createShaderModule(const std::vector<char>& code);
    // std::vector<char> readShaderFile(const std::string& filename);
    std::unique_ptr<PipelineViewportState> viewportState;
    std::unique_ptr<PipelineRasterizerState> rasterizationState;
    std::unique_ptr<PipelineMultisampleState> multisampleState;
    std::unique_ptr<PipelineColorBlendState> colorBlendState;
    std::unique_ptr<PipelineDepthStencilState> depthStencilState;
    std::unique_ptr<PipelineLayout> pipelineLayoutState;
    std::unique_ptr<PipelineVertexInputState> vertexInputState;

    std::unique_ptr<PipelineShaderModule> vertexShaderModule;
    std::unique_ptr<PipelineShaderModule> fragmentShaderModule;

    //VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
    //VkPipelineVertexInputStateCreateInfo createVertexInputState();
    VkPipelineInputAssemblyStateCreateInfo createInputAssemblyState();
    //VkPipelineRasterizationStateCreateInfo createRasterizerState();
    //VkPipelineMultisampleStateCreateInfo createMultisampleState();
    //VkPipelineColorBlendStateCreateInfo createColorBlendState();
    //VkPipelineLayoutCreateInfo createPipelineLayoutInfo();
    //VkPipelineDepthStencilStateCreateInfo createDepthStencilState();
    //VkPipelineViewportStateCreateInfo createViewportState();
    VkPipelineDynamicStateCreateInfo createDynamicState();
};
