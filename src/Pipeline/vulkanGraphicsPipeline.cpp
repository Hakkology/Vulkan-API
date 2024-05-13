#include "vulkanGraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent)
: device(device), renderPass(renderPass), swapChainExtent(swapChainExtent) {}

GraphicsPipeline::~GraphicsPipeline() {}

void GraphicsPipeline::createGraphicsPipeline(const std::string &vertShaderPath, const std::string &fragShaderPath)
{
    // Read in SPIR-V code of shaders.
    auto vertexShaderCode = readShaderFile(vertShaderPath);
    auto fragmentShaderCode = readShaderFile(fragShaderPath);

    // Build Shader Modules to link to graphics pipeline.
    VkShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
    VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

    // Put shader stage creation info in to array.
    // graphics pipeline creation info requires array of shader stage creates
    VkPipelineShaderStageCreateInfo shaderStages[] = {
        createShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShaderModule),
        createShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderModule)
    };

    // Create pipeline
    std::cout << "Creating Pipeline..." << std::endl;
    createVertexInputState();
    createInputAssemblyState();
    createViewportState();
    createDynamicState();
    createRasterizerState();
    createMultisampleState();
    createColorBlendState();
    
    VkResult result = vkCreatePipelineLayout(device, &createPipelineLayoutInfo(), nullptr, &pipelineLayout);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Pipeline Layout");
    }
    
    createDepthStencilState();

    // Destroy shader modules, no longer needed after pipeline is created.
    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);
}

void GraphicsPipeline::cleanup()
{
    if (pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        pipelineLayout = VK_NULL_HANDLE;
        std::cout << "Pipeline layout destroyed." << std::endl;
    }

    if (graphicsPipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
        graphicsPipeline = VK_NULL_HANDLE;
        std::cout << "Graphics pipeline destroyed." << std::endl;
    }
}

VkShaderModule GraphicsPipeline::createShaderModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = code.size();                                      // size of code.
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());      // pointer to code.

    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);
    if (result!= VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create a shader module.");
    }

    return shaderModule;
    
}

std::vector<char> GraphicsPipeline::readShaderFile(const std::string &filename)
{
    // Open stream from given file
    // std::ios::binary tells stream to read file as binary
    // std::ios::ate tells stream to start reading from the eof
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    // Get current read position and use to resize file buffer
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> fileBuffer(fileSize);

    // Move read position to start of file and read the file data completely.
    file.seekg(0);
    file.read(fileBuffer.data(), fileSize);
    file.close();

    return fileBuffer;
}

VkPipelineShaderStageCreateInfo GraphicsPipeline::createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
{
    // Shader stage creation information --
    VkPipelineShaderStageCreateInfo shaderStageInfo = {};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;                      // Specify the pipeline stage the shader belongs to.
    shaderStageInfo.module = shaderModule;              // Specify the shader module to use.
    shaderStageInfo.pName = "main";                     // Entry point of the shader; "main" by default.

    return shaderStageInfo;
}

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

VkPipelineRasterizationStateCreateInfo GraphicsPipeline::createRasterizerState()
{
    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
    rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerCreateInfo.depthClampEnable = VK_FALSE;                                           // Change if fragments beyond near/far planes are clipped or clamped.
    // Device features in logical device must be enabled if this is toggled on.
    rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;                                    // Whether to discard data and skip rasterizer. NEver creaters fragments, suitable for pipelines without graphics.
    rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;                                    // How to handle filling points between vertices.
    // GPU features are needed for some.
    rasterizerCreateInfo.lineWidth = 1.0f;                                                      // How thick lines should be when drawn.
    rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;                                      // Culling, whether if both sides are rendered.
    rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;                                   // Winding to determine which side is front.
    rasterizerCreateInfo.depthBiasEnable = VK_FALSE;                                            // Whether to add depth bias to fragments for shadow acne fix.

    return rasterizerCreateInfo;
}

VkPipelineMultisampleStateCreateInfo GraphicsPipeline::createMultisampleState()
{
    // Multi sampling
    VkPipelineMultisampleStateCreateInfo multiSamplingCreateInfo = {};
    multiSamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multiSamplingCreateInfo.sampleShadingEnable = VK_FALSE;                                     // Enable multi sampling shading or not.
    multiSamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;                       // Number of samples to use per fragment.

    return multiSamplingCreateInfo;
}

VkPipelineColorBlendStateCreateInfo GraphicsPipeline::createColorBlendState()
{
    // Blending
    // Blending decides how to blend a new colour being written to a fragment with the old value.

    // Blend attachment State (how blending is handled)
    VkPipelineColorBlendAttachmentState colourState = {};                                       // Colours to apply blending to.
    colourState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colourState.blendEnable = VK_TRUE;                                                          // Enable blending
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
}

VkPipelineDepthStencilStateCreateInfo GraphicsPipeline::createDepthStencilState()
{
    // Depth stencil testing.
    // TODO: Set up depth stencil testing.
}

VkPipelineViewportStateCreateInfo GraphicsPipeline::createViewportState()
{
    // Viewport & Scissor
    // Create a viewport
    VkViewport viewport = {};
    viewport.x = 0.0f;                                                                      // x Start coordinates
    viewport.y = 0.0f;                                                                      // y Start coordinate.
    viewport.width = (float) swapChainExtent.width;                                         // viewport width
    viewport.height = (float) swapChainExtent.height;                                       // viewport height
    viewport.minDepth = 0.0f;                                                               // min framebuffer depth
    viewport.maxDepth = 1.0f;                                                               // max framebuffer depth

    // Create a scissor info struct
    VkRect2D scissor = {};  
    scissor.offset = {0,0};                                                                 // offset to use region from
    scissor.extent = swapChainExtent;                                                       // extent to describe region to use, starting at offset
    
    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;

    return viewportStateCreateInfo;
}

VkPipelineDynamicStateCreateInfo GraphicsPipeline::createDynamicState()
{
    // Dynamic State
    // Dynamic states to enable:
    // std::vector<VkDynamicState> dynamicStateEnables;
    // dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);                               // Dynamic viewport: can resize in command buffer with vkCmdSetViewport(commandbuffer, 0, 1, &viewport)
    // dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);                                // Dynamic scissor: can resize in command buffer with vkCmdSetScissor(commandbuffer, 0 ,1, &scissor)
    // // Do not destroy the pipeline but just pass the updated size to a new image on swapchain.

    // VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
    // dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    // dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();

    // return dynamicStateCreateInfo;
}
