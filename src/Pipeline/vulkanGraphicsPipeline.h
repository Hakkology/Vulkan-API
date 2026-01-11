#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "pipelineColorBlendState.h"
#include "pipelineDepthStencilState.h"
#include "pipelineDynamicState.h"
#include "pipelineInputAssemblyState.h"
#include "pipelineLayoutState.h"
#include "pipelineMultiSampleState.h"
#include "pipelineRasterizerState.h"
#include "pipelineShaderModule.h"
#include "pipelineVertexInputState.h"
#include "pipelineViewportState.h"

class GraphicsPipeline {
public:
  GraphicsPipeline(VkDevice device, VkRenderPass renderPass,
                   VkExtent2D swapChainExtent,
                   const std::string &vertexShaderPath,
                   const std::string &fragmentShaderPath,
                   VkCullModeFlags cullMode = VK_CULL_MODE_NONE,
                   VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE,
                   VkBool32 depthBiasEnable = VK_FALSE);
  ~GraphicsPipeline();

  void createGraphicsPipeline(
      const std::vector<VkDescriptorSetLayout> &descriptorSetLayouts);
  void cleanup();
  void resetPipelineUnits();

  VkPipeline getGraphicsPipeline() const { return graphicsPipeline; }
  VkPipelineLayout getPipelineLayout() const {
    return pipelineLayoutState->getPipelineLayout();
  }
  PipelineLayout *getLayoutState() const { return pipelineLayoutState.get(); }

private:
  VkPipeline graphicsPipeline;
  VkDevice device;
  VkExtent2D swapChainExtent;
  VkRenderPass renderPass;

  std::string vertexShaderPath;
  std::string fragmentShaderPath;

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

  // Rasterizer options
  VkCullModeFlags cullMode;
  VkFrontFace frontFace;
  VkBool32 depthBiasEnable;
};
