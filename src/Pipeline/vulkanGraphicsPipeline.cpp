#include "vulkanGraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(VkDevice device, VkRenderPass& renderPass, VkExtent2D& swapChainExtent)
: device(device), renderPass(renderPass), swapChainExtent(swapChainExtent) {

}

GraphicsPipeline::~GraphicsPipeline() {
    cleanup();
}

void GraphicsPipeline::createGraphicsPipeline()
{
    resetPipelineUnits();
    // Read in SPIR-V code of shaders.
    // Build Shader Modules to link to graphics pipeline.
    // Put shader stage creation info in to array.
    // graphics pipeline creation info requires array of shader stage creates
    vertexShaderModule = std::make_unique<PipelineShaderModule>(device, VK_SHADER_STAGE_VERTEX_BIT, VERTEX_SHADER_PATH);
    fragmentShaderModule = std::make_unique<PipelineShaderModule>(device, VK_SHADER_STAGE_FRAGMENT_BIT, FRAGMENT_SHADER_PATH);

    auto vertShaderStageInfo = vertexShaderModule->createShaderStageInfo();
    auto fragShaderStageInfo = fragmentShaderModule->createShaderStageInfo();

    std::cout << "Shader stages are being set." << std::endl;
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // Create pipeline
    std::cout << "Creating Vertex Input State..." << std::endl;
    vertexInputState = std::make_unique<PipelineVertexInputState>();
    const VkPipelineVertexInputStateCreateInfo* vertexInputStateInfo =  vertexInputState->getVertexInputState();

    std::cout << "Creating Input Assembly State..." << std::endl;
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssemblyState();

    std::cout << "Creating Viewport State..." << std::endl;
    viewportState = std::make_unique<PipelineViewportState>(device, swapChainExtent);
    const VkPipelineViewportStateCreateInfo* viewportStateInfo = viewportState->getViewportStateCreateInfo();

    std::cout << "Creating Dynamic State..." << std::endl;
    VkPipelineDynamicStateCreateInfo dynamicState = createDynamicState();

    std::cout << "Creating Rasterization State..." << std::endl;
    rasterizationState = std::make_unique<PipelineRasterizerState>();
    const VkPipelineRasterizationStateCreateInfo* rasterizationStateInfo = rasterizationState->getRasterizationStateInfo();

    std::cout << "Creating Multisampling State..." << std::endl;
    multisampleState = std::make_unique<PipelineMultisampleState>();
    const VkPipelineMultisampleStateCreateInfo* multisampleStateInfo = multisampleState->getMultisampleState();

    std::cout << "Creating Color Blend State..." << std::endl;
    colorBlendState = std::make_unique<PipelineColorBlendState>(device);
    const VkPipelineColorBlendStateCreateInfo* colorBlendStateInfo = colorBlendState->getColorBlendStateInfo();

    std::cout << "Creating pipeline layout info..." << std::endl;
    pipelineLayoutState = std::make_unique<PipelineLayout>(device);
    const VkPipelineLayoutCreateInfo* pipelineLayoutInfo = pipelineLayoutState->getLayoutInfo();
    const VkPipelineLayout pipelineLayout = pipelineLayoutState->getPipelineLayout();
    
    std::cout << "Creating depth stencil info..." << std::endl;
    depthStencilState = std::make_unique<PipelineDepthStencilState>(device);
    const VkPipelineDepthStencilStateCreateInfo* depthStencilInfo = depthStencilState->getDepthStencilStateInfo();

    // Creating graphics pipeline.
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;                              // number of shader stages
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = vertexInputStateInfo;       // All the fixed function pipeline states
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = viewportStateInfo;
    pipelineCreateInfo.pDynamicState = nullptr;
    pipelineCreateInfo.pRasterizationState = rasterizationStateInfo;
    pipelineCreateInfo.pMultisampleState = multisampleStateInfo;
    pipelineCreateInfo.pColorBlendState = colorBlendStateInfo;
    pipelineCreateInfo.pDepthStencilState = depthStencilInfo;
    pipelineCreateInfo.layout = pipelineLayout;                     // pipeline layout pipeline should use.
    pipelineCreateInfo.renderPass = renderPass;                     // render pass description the pipeline is compatible with
    pipelineCreateInfo.subpass = 0;                                 // subpass of render pass to use with pipeline
    // pipeline derivatives : can create multiple pipelines that derive from one another for optimization.
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;         // Existing pipeline to derive from...
    pipelineCreateInfo.basePipelineIndex = -1;                      // index of pipeline being created to derive from if multiple pipelines are being created.
    // You may as well have a pipeline which is 0 and other pipelines can simply be varied from that 0.

    // cache helps you save that data and make new ones from it.

    // Create graphics pipeline.
    std::cout << "Creating graphics pipeline..." << std::endl;
    bool result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
    if (result != VK_SUCCESS)
        throw std::runtime_error("Failed to create a graphics pipeline.");
    
    std::cout << "Graphics pipeline create successfully..." << std::endl;
}

void GraphicsPipeline::cleanup()
{
    if (graphicsPipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        graphicsPipeline = VK_NULL_HANDLE;
        std::cout << "Graphics pipeline destroyed." << std::endl;
    }

    resetPipelineUnits();
}

void GraphicsPipeline::resetPipelineUnits()
{
    if (vertexShaderModule) {
        vertexShaderModule.reset();
        std::cout << "Vertex shader module destroyed." << std::endl;
    }

    if (fragmentShaderModule) {
        fragmentShaderModule.reset();
        std::cout << "Fragment shader module destroyed." << std::endl;
    }

    if (vertexInputState) {
        vertexInputState.reset();
        std::cout << "Vertex Input state destroyed." << std::endl;
    }

    if (viewportState) {
        viewportState.reset();
        std::cout << "Viewport state destroyed." << std::endl;
    }

    if (rasterizationState) {
        rasterizationState.reset();
        std::cout << "Rasterization state destroyed." << std::endl;
    }

    if (multisampleState) {
        multisampleState.reset();
        std::cout << "Multisample state destroyed." << std::endl;
    }

    if (colorBlendState) {
        colorBlendState.reset();
        std::cout << "Color Blend state destroyed." << std::endl;
    }

    if (depthStencilState) {
        depthStencilState.reset();
        std::cout << "Depth Stencil state destroyed." << std::endl;
    }

    if (pipelineLayoutState) {
        pipelineLayoutState.reset();
        std::cout << "Depth Stencil state destroyed." << std::endl;
    }
}

// // Vertex Input, we should put in vertex descriptions when resources are created.
// VkPipelineVertexInputStateCreateInfo GraphicsPipeline::createVertexInputState()
// {
//     // Vertex Input 
//     VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
//     vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//     vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
//     vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;                             // list of vertex binding descriptions (data spacing & stride information)
//     vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;                          
//     vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;                           // list of vertex attribute descriptions (data format and where to bind to/from)

//     return vertexInputCreateInfo;
// }

VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::createInputAssemblyState()
{
    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = {};
    InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;                           // Primitive type to assemble vertices as.
    InputAssembly.primitiveRestartEnable = VK_FALSE;                                        // Allow overriding of "strip" topology to start new primitives.

    return InputAssembly;
}

VkPipelineDynamicStateCreateInfo GraphicsPipeline::createDynamicState()
{
    // Dynamic State
    // Dynamic states to enable:
    // std::vector<VkDynamicState> dynamicStateEnables;
    // dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);                               // Dynamic viewport: can resize in command buffer with vkCmdSetViewport(commandbuffer, 0, 1, &viewport)
    // dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);                                // Dynamic scissor: can resize in command buffer with vkCmdSetScissor(commandbuffer, 0 ,1, &scissor)
    // // Do not destroy the pipeline but just pass the updated size to a new image on swapchain.

    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
    // dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    // dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();

    return dynamicStateCreateInfo;
}
