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

    // Shader stage creation information --
    // Vertex stage creation info

    VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
    vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;                          // Shader stage bane
    vertexShaderCreateInfo.module = vertexShaderModule;                                 // Shader module used by stage
    vertexShaderCreateInfo.pName = "main";                                              // Entry point to shader

    VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
    fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;                          // Shader stage bane
    fragmentShaderCreateInfo.module = fragmentShaderModule;                                 // Shader module used by stage
    fragmentShaderCreateInfo.pName = "main";                                              // Entry point to shader
    
    // Put shader stage creation info in to array.
    // graphics pipeline creation info requires array of shader stage creates
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

    // Create pipeline
    std::cout << "Creating Pipeline..." << std::endl;

    

    // Destroy shader modules, no longer needed after pipeline is created.
    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);
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
