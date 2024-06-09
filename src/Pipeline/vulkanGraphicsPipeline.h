#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "pipelineShaderModule.h"
#include "pipelineVertexInputState.h"
#include "pipelineInputAssemblyState.h"
#include "pipelineViewportState.h"
#include "pipelineRasterizerState.h"
#include "pipelineMultiSampleState.h"
#include "pipelineColorBlendState.h"
#include "pipelineDepthStencilState.h"
#include "pipelineLayoutState.h"
#include "pipelineDynamicState.h"

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
    VkDevice device;
    VkExtent2D swapChainExtent;
    VkRenderPass& renderPass;

    std::unique_ptr<PipelineViewportState> viewportState;
    std::unique_ptr<PipelineVertexInputState> vertexInputState;
    std::unique_ptr<PipelineInputAssemblyState> inputAssemblyState;
    std::unique_ptr<PipelineRasterizerState> rasterizationState;
    std::unique_ptr<PipelineMultisampleState> multisampleState;
    std::unique_ptr<PipelineColorBlendState> colorBlendState;
    std::unique_ptr<PipelineDepthStencilState> depthStencilState;
    std::unique_ptr<PipelineLayout> pipelineLayoutState;
    std::unique_ptr<PipelineDynamicState> dynamicState;

    std::unique_ptr<PipelineShaderModule> vertexShaderModule;
    std::unique_ptr<PipelineShaderModule> fragmentShaderModule;
};
