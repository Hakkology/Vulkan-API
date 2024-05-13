#pragma once

#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

static constexpr char const* VERTEX_SHADER_PATH = "../src/Shaders/spv/shader.vert.spv";
static constexpr char const* FRAGMENT_SHADER_PATH = "../src/Shaders/spv/shader.frag.spv";

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
    VkPipelineColorBlendAttachmentState createColorBlendAttachment();
    VkPipelineColorBlendStateCreateInfo createColorBlendState();
    VkPipelineLayoutCreateInfo createPipelineLayoutInfo();
    VkPipelineDepthStencilStateCreateInfo createDepthStencilState();
    VkPipelineViewportStateCreateInfo createViewportState();
    VkPipelineDynamicStateCreateInfo createDynamicState();
};
