#include "vulkanGraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(
    VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent,
    const std::string &vertShaderPath, const std::string &fragShaderPath,
    VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBiasEnable)
    : device(device), renderPass(renderPass), swapChainExtent(swapChainExtent),
      vertexShaderPath(vertShaderPath), fragmentShaderPath(fragShaderPath),
      cullMode(cullMode), frontFace(frontFace),
      depthBiasEnable(depthBiasEnable) {}

GraphicsPipeline::~GraphicsPipeline() { cleanup(); }

void GraphicsPipeline::createGraphicsPipeline(
    const std::vector<VkDescriptorSetLayout> &descriptorSetLayouts) {
  resetPipelineUnits();

  vertexShaderModule = std::make_unique<PipelineShaderModule>(
      device, VK_SHADER_STAGE_VERTEX_BIT, vertexShaderPath);
  fragmentShaderModule = std::make_unique<PipelineShaderModule>(
      device, VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderPath);

  auto vertShaderStageInfo = vertexShaderModule->createShaderStageInfo();
  auto fragShaderStageInfo = fragmentShaderModule->createShaderStageInfo();

  std::cout << "Shader stages are being set." << std::endl;
  VkPipelineShaderStageCreateInfo shaderStagesInfo[] = {vertShaderStageInfo,
                                                        fragShaderStageInfo};

  // Create pipeline
  std::cout << "Creating Vertex Input State..." << std::endl;
  vertexInputState = std::make_unique<PipelineVertexInputState>(device);
  const VkPipelineVertexInputStateCreateInfo *vertexInputStateInfo =
      vertexInputState->getVertexInputState();

  std::cout << "Creating Input Assembly State..." << std::endl;
  inputAssemblyState = std::make_unique<PipelineInputAssemblyState>();
  const VkPipelineInputAssemblyStateCreateInfo *inputAssemblyStateInfo =
      inputAssemblyState->getInputAssemblyStateInfo();

  std::cout << "Creating Viewport State..." << std::endl;
  viewportState =
      std::make_unique<PipelineViewportState>(device, swapChainExtent);
  const VkPipelineViewportStateCreateInfo *viewportStateInfo =
      viewportState->getViewportStateCreateInfo();

  std::cout << "Creating Dynamic State..." << std::endl;
  dynamicState = std::make_unique<PipelineDynamicState>();
  const VkPipelineDynamicStateCreateInfo *dynamicStateInfo =
      dynamicState->getDynamicStateInfo();

  std::cout << "Creating Rasterization State..." << std::endl;
  rasterizationState = std::make_unique<PipelineRasterizerState>(
      cullMode, frontFace, depthBiasEnable);
  const VkPipelineRasterizationStateCreateInfo *rasterizationStateInfo =
      rasterizationState->getRasterizationStateInfo();

  std::cout << "Creating Multisampling State..." << std::endl;
  multisampleState = std::make_unique<PipelineMultisampleState>();
  const VkPipelineMultisampleStateCreateInfo *multisampleStateInfo =
      multisampleState->getMultisampleState();

  std::cout << "Creating Color Blend State..." << std::endl;
  colorBlendState = std::make_unique<PipelineColorBlendState>(device);
  const VkPipelineColorBlendStateCreateInfo *colorBlendStateInfo =
      colorBlendState->getColorBlendStateInfo();

  std::cout << "Configuring pipeline layout..." << std::endl;
  // Use the passed descriptor set layouts for texture binding
  pipelineLayoutState =
      std::make_unique<PipelineLayout>(device, descriptorSetLayouts);
  if (!pipelineLayoutState->createPipelineLayout()) {
    // Handle error, e.g., throw an exception or return false
    // For now, let's assume it's a void function and just return if it fails
    return; // Or throw std::runtime_error("Failed to create pipeline layout.");
  }
  const VkPipelineLayoutCreateInfo *pipelineLayoutInfo =
      pipelineLayoutState->getLayoutInfo();
  const VkPipelineLayout pipelineLayout =
      pipelineLayoutState->getPipelineLayout();

  std::cout << "Creating depth stencil info..." << std::endl;
  depthStencilState = std::make_unique<PipelineDepthStencilState>(device);
  const VkPipelineDepthStencilStateCreateInfo *depthStencilInfo =
      depthStencilState->getDepthStencilStateInfo();

  // Creating graphics pipeline.
  VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
  pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineCreateInfo.stageCount = 2; // number of shader stages
  pipelineCreateInfo.pStages = shaderStagesInfo;
  pipelineCreateInfo.pVertexInputState =
      vertexInputStateInfo; // All the fixed function pipeline states
  pipelineCreateInfo.pInputAssemblyState = inputAssemblyStateInfo;
  pipelineCreateInfo.pViewportState = viewportStateInfo;
  pipelineCreateInfo.pDynamicState = dynamicStateInfo;
  pipelineCreateInfo.pRasterizationState = rasterizationStateInfo;
  pipelineCreateInfo.pMultisampleState = multisampleStateInfo;
  pipelineCreateInfo.pColorBlendState = colorBlendStateInfo;
  pipelineCreateInfo.pDepthStencilState = depthStencilInfo;
  pipelineCreateInfo.layout =
      pipelineLayout; // pipeline layout pipeline should use.
  pipelineCreateInfo.renderPass =
      renderPass; // render pass description the pipeline is compatible with
  pipelineCreateInfo.subpass = 0; // subpass of render pass to use with pipeline
  // pipeline derivatives : can create multiple pipelines that derive from one
  // another for optimization.
  pipelineCreateInfo.basePipelineHandle =
      VK_NULL_HANDLE; // Existing pipeline to derive from...
  pipelineCreateInfo.basePipelineIndex =
      -1; // index of pipeline being created to derive from if multiple
          // pipelines are being created.
  // You may as well have a pipeline which is 0 and other pipelines can simply
  // be varied from that 0. cache helps you save that data and make new ones
  // from it.

  // Create graphics pipeline.
  std::cout << "Creating graphics pipeline..." << std::endl;
  bool result =
      vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
                                nullptr, &graphicsPipeline);
  if (result != VK_SUCCESS)
    throw std::runtime_error("Failed to create a graphics pipeline.");

  std::cout << "Graphics pipeline create successfully..." << std::endl;
}

void GraphicsPipeline::cleanup() {
  if (graphicsPipeline != VK_NULL_HANDLE) {
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    graphicsPipeline = VK_NULL_HANDLE;
    std::cout << "Graphics pipeline destroyed." << std::endl;
  }

  resetPipelineUnits();
}

void GraphicsPipeline::resetPipelineUnits() {
  if (vertexShaderModule)
    vertexShaderModule.reset();

  if (fragmentShaderModule)
    fragmentShaderModule.reset();

  if (vertexInputState)
    vertexInputState.reset();

  if (inputAssemblyState)
    inputAssemblyState.reset();

  if (viewportState)
    viewportState.reset();

  if (rasterizationState)
    rasterizationState.reset();

  if (multisampleState)
    multisampleState.reset();

  if (colorBlendState)
    colorBlendState.reset();

  if (depthStencilState)
    depthStencilState.reset();

  if (pipelineLayoutState)
    pipelineLayoutState.reset();

  if (dynamicState)
    dynamicState.reset();

  std::cout << "All pipeline units are reset." << std::endl;
}
