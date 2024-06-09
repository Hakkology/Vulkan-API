#include "vulkanGraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(VkDevice device, VkRenderPass& renderPass, VkExtent2D& swapChainExtent)
: device(device), renderPass(renderPass), swapChainExtent(swapChainExtent) {
    std::cout << "Initialized Graphics Pipeline with swapChainExtent: " << swapChainExtent.width << "x" << swapChainExtent.height << std::endl;
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
    VkPipelineVertexInputStateCreateInfo vertexInputState =  createVertexInputState();

    std::cout << "Creating Input Assembly State..." << std::endl;
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssemblyState();

    std::cout << "Creating Viewport State..." << std::endl;
    //VkPipelineViewportStateCreateInfo viewportStateInfo = createViewportState();
    viewportState = std::make_unique<PipelineViewportState>(device, swapChainExtent);
    const VkPipelineViewportStateCreateInfo* viewportStateInfo = viewportState->getViewportStateCreateInfo();

    std::cout << "Creating Dynamic State..." << std::endl;
    VkPipelineDynamicStateCreateInfo dynamicState = createDynamicState();

    std::cout << "Creating Rasterization State..." << std::endl;
    //VkPipelineRasterizationStateCreateInfo rasterizationState = createRasterizerState();
    rasterizationState = std::make_unique<PipelineRasterizerState>();
    const VkPipelineRasterizationStateCreateInfo* rasterizationStateInfo = rasterizationState->getRasterizationStateInfo();

    std::cout << "Creating Multisampling State..." << std::endl;
    //VkPipelineMultisampleStateCreateInfo multiSamplingState = createMultisampleState();
    multisampleState = std::make_unique<PipelineMultisampleState>();
    const VkPipelineMultisampleStateCreateInfo* multisampleStateInfo = multisampleState->getMultisampleState();

    std::cout << "Creating Color Blend State..." << std::endl;
    VkPipelineColorBlendStateCreateInfo colorBlendState = createColorBlendState();

    std::cout << "Creating pipeline layout info..." << std::endl;
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = createPipelineLayoutInfo();
    
    VkResult result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Pipeline Layout");
    }
    std::cout << "Graphics Pipeline configured successfully." << std::endl;
    
    std::cout << "Creating depth stencil info..." << std::endl;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = createDepthStencilState();

    // Creating graphics pipeline.
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;                              // number of shader stages
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &vertexInputState;       // All the fixed function pipeline states
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = viewportStateInfo;
    pipelineCreateInfo.pDynamicState = nullptr;
    pipelineCreateInfo.pRasterizationState = rasterizationStateInfo;
    pipelineCreateInfo.pMultisampleState = multisampleStateInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilInfo;
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
    result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create a graphics pipeline.");
    }

    std::cout << "Graphics pipeline create successfully..." << std::endl;
}

void GraphicsPipeline::cleanup()
{
    if (graphicsPipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        graphicsPipeline = VK_NULL_HANDLE;
        std::cout << "Graphics pipeline destroyed." << std::endl;
    }

    if (pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        pipelineLayout = VK_NULL_HANDLE;
        std::cout << "Pipeline layout destroyed." << std::endl;
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
}

// Vertex Input, we should put in vertex descriptions when resources are created.
VkPipelineVertexInputStateCreateInfo GraphicsPipeline::createVertexInputState()
{
    // Vertex Input 
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
    vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;                             // list of vertex binding descriptions (data spacing & stride information)
    vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;                          
    vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;                           // list of vertex attribute descriptions (data format and where to bind to/from)

    return vertexInputCreateInfo;
}

VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::createInputAssemblyState()
{
    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = {};
    InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;                           // Primitive type to assemble vertices as.
    InputAssembly.primitiveRestartEnable = VK_FALSE;                                        // Allow overriding of "strip" topology to start new primitives.

    return InputAssembly;
}

// VkPipelineRasterizationStateCreateInfo GraphicsPipeline::createRasterizerState()
// {
//     // Rasterizer
//     VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
//     rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//     rasterizerCreateInfo.depthClampEnable = VK_FALSE;                                           // Change if fragments beyond near/far planes are clipped or clamped.
//     // Device features in logical device must be enabled if this is toggled on.
//     rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;                                    // Whether to discard data and skip rasterizer. NEver creaters fragments, suitable for pipelines without graphics.
//     rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;                                    // How to handle filling points between vertices.
//     // GPU features are needed for some.
//     rasterizerCreateInfo.lineWidth = 1.0f;                                                      // How thick lines should be when drawn.
//     rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;                                      // Culling, whether if both sides are rendered.
//     rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;                                   // Winding to determine which side is front.
//     rasterizerCreateInfo.depthBiasEnable = VK_FALSE;                                            // Whether to add depth bias to fragments for shadow acne fix.

//     return rasterizerCreateInfo;
// }

// VkPipelineMultisampleStateCreateInfo GraphicsPipeline::createMultisampleState()
// {
//     // Multi sampling
//     VkPipelineMultisampleStateCreateInfo multiSamplingCreateInfo = {};
//     multiSamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//     multiSamplingCreateInfo.sampleShadingEnable = VK_FALSE;                                     // Enable multi sampling shading or not.
//     multiSamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;                       // Number of samples to use per fragment.

//     return multiSamplingCreateInfo;
// }

// VkPipelineViewportStateCreateInfo GraphicsPipeline::createViewportState() {
//     // Viewport & Scissor

//     // Create a viewport
//     viewport.x = 0.0f;                                                                      // x Start coordinates
//     viewport.y = 0.0f;                                                                      // y Start coordinate.
//     viewport.width = static_cast<float> (swapChainExtent.width);                                         // viewport width
//     viewport.height = static_cast<float> (swapChainExtent.height);                                       // viewport height
//     viewport.minDepth = 0.0f;                                                               // min framebuffer depth
//     viewport.maxDepth = 1.0f;                                                               // max framebuffer depth

//     // Scissor configuration
//     scissor.offset = {0,0};                                                                 // offset to use region from
//     scissor.extent = swapChainExtent;                                                       // extent to describe region to use, starting at offset

//     std::cout << "Scissor - Offset X: " << scissor.offset.x << ", Offset Y: " << scissor.offset.y << ", Width: " << scissor.extent.width << ", Height: " << scissor.extent.height << std::endl;

//     VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
//     viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//     viewportStateCreateInfo.viewportCount = 1;
//     viewportStateCreateInfo.pViewports = &viewport;
//     viewportStateCreateInfo.scissorCount = 1;
//     viewportStateCreateInfo.pScissors = &scissor;

//     return viewportStateCreateInfo;
// }

VkPipelineColorBlendStateCreateInfo GraphicsPipeline::createColorBlendState()
{
    // Blending
    // Blending decides how to blend a new colour being written to a fragment with the old value.

    // Blend attachment State (how blending is handled)
    colourState = {};                                       // Colours to apply blending to.
    colourState.blendEnable = VK_FALSE;                                                          // Enable blending

    // Blending uses equation: (srcCOlorBlendFactor * new colour) colorBlendOp (dstColorBlendFactor * old colour)
    colourState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colourState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colourState.colorBlendOp = VK_BLEND_OP_ADD;
    // Summarized: (VK_BLEND_FACTOR_SRC_ALPHA * new colour) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * old colour)
    //              (new colour alpha * new colour) + ((1- new colour alpha) * old colour)
    colourState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colourState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colourState.alphaBlendOp = VK_BLEND_OP_ADD;
    // Summarized: (1* newalpha) + (0* oldalpha)
    colourState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colourBlendingCreateInfo = {};
    colourBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colourBlendingCreateInfo.logicOpEnable = VK_FALSE;                                          // Alternative to calculations is to use logical operations.
    colourBlendingCreateInfo.attachmentCount = 1;
    colourBlendingCreateInfo.pAttachments = &colourState;

    return colourBlendingCreateInfo;
}

VkPipelineLayoutCreateInfo GraphicsPipeline::createPipelineLayoutInfo()
{
    // Pipeline Layout (TODO: Apply future descriptor set layouts)
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.pSetLayouts = nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;   

    return pipelineLayoutCreateInfo;
}

VkPipelineDepthStencilStateCreateInfo GraphicsPipeline::createDepthStencilState()
{
    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo;
    // Depth stencil testing.
    // TODO: Set up depth stencil testing.
    return depthStencilStateInfo;
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
