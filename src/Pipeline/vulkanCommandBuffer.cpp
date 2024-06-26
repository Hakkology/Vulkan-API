#include "vulkanCommandBuffer.h"


CommandManager::CommandManager(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkPipeline graphicsPipeline)
: device(device), physicalDevice(physicalDevice), surface(surface), graphicsPipeline(graphicsPipeline) {}

CommandManager::~CommandManager() {}

void CommandManager::createCommandPool()
{
    std::cout << "Command pool being configured." << std::endl;
    
    // Get indices of queue families from device.
    QueueFamilyIndices queueFamilyIndices = VulkanUtils::findQueueFamiliesForSurface(physicalDevice, surface);

    if (!queueFamilyIndices.isValid()) {
        std::cerr << "Invalid queue family indices. Graphics: " << queueFamilyIndices.graphicsFamily
                  << ", Presentation: " << queueFamilyIndices.presentationFamily << std::endl;
        return;
    }

    // Check the address of queueFamilyIndices to see if it's accessible
    std::cout << "Address of queueFamilyIndices: " << &queueFamilyIndices << std::endl;

    // Graphics family queue index.
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = 0;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;      // Queue family type that buffers from this command pool will use.

    std::cout << "Using queue family index: " << poolInfo.queueFamilyIndex << std::endl;

    // Create a graphics queue family command pool.
    VkResult result = vkCreateCommandPool(device, &poolInfo, nullptr, &graphicsCommandPool);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create a command pool. Error code: " << result << std::endl;
        throw std::runtime_error("Failed to create a command pool.");
    }

    std::cout << "Command buffers configured successfully." << std::endl;
    
}

void CommandManager::allocateCommandBuffers(std::vector<VkFramebuffer> frameBuffers)
{
    std::cout << "Command buffers are being allocated." << std::endl;
    // Resize command buffer count to have one for each framebuffer
    commandBuffers.resize(frameBuffers.size());

    VkCommandBufferAllocateInfo commandbufferAllocateInfo = {};
    commandbufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandbufferAllocateInfo.commandPool = graphicsCommandPool;
    commandbufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;              // vkCmdExecuteCommands (buffer)
    // primary means you pass it to queue, secondary means you pass it to a different commandbuffer.
    commandbufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    
    // allocate command buffers and place handles in array of buffers.
    VkResult result = vkAllocateCommandBuffers(device, &commandbufferAllocateInfo, commandBuffers.data());
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers.");
    }
    

    std::cout << "Command buffers are allocated successfully." << std::endl;
}

void CommandManager::recordCommands(std::vector<VkFramebuffer> frameBuffers, VkRenderPass& renderPass, VkExtent2D chosenExtent, 
                                    MeshDrawer* meshDrawer, MeshManager* meshManager)
{
    std::cout << "Command recording has begun." << std::endl;
    // Information about how to begin each command buffer.
    VkCommandBufferBeginInfo bufferBeginInfo = {};
    bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    //bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;       // buffer can be resubmitted when it has already been submitted and is awaiting execution.
    // Fences now control this buffer begin info mechanism.
    
    // Information about how to begin a render pass (only needed for graphical applications)
    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass;                                // render pass to begin.
    renderPassBeginInfo.renderArea = {0, 0};                                    // start point of render pass in pixels.
    renderPassBeginInfo.renderArea.extent = chosenExtent;                       // size of region to run render pass on (starting at offset)

    VkClearValue clearValues[]={{.6f, .65f, .4f, 1.0f}};
    renderPassBeginInfo.pClearValues = clearValues;                              // list of clear values (TODO: Depth attachment clear value)
    renderPassBeginInfo.clearValueCount = 1;

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        std::cout << "Setting up framebuffer " << i << " for command recording." << std::endl;
        renderPassBeginInfo.framebuffer = frameBuffers[i];
        // Start recording commands to command buffer.
        VkResult result = vkBeginCommandBuffer(commandBuffers[i], &bufferBeginInfo);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to start recording a command buffer.");
        }

        // anything with vkCmd are commands that are being recorded.
        // all our render pass commands are primary pipeline.
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        for (auto& meshPair : meshManager->getAllMeshes()) {
            meshDrawer->drawMesh(commandBuffers[i], meshPair.second.get());
        }
        vkCmdEndRenderPass(commandBuffers[i]);          // end render pass

        // Stop recording the command buffer.
        result = vkEndCommandBuffer(commandBuffers[i]);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to stop recording a command buffer.");
        }
    }
    
    std::cout << "Command buffers are recorded successfully." << std::endl;
    //vkBeginCommandBuffer()
}

void CommandManager::cleanup() {

    // Check if command buffers exist and need to be freed
    if (!commandBuffers.empty() && graphicsCommandPool != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(device, graphicsCommandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear(); // Clear the container of command buffers
        std::cout << "Command buffers freed." << std::endl;
    }

    // Destroy the command pool
    if (graphicsCommandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(device, graphicsCommandPool, nullptr);
        graphicsCommandPool = VK_NULL_HANDLE; // Set to VK_NULL_HANDLE after destruction
        std::cout << "Command pool destroyed." << std::endl;
    }
}

VkCommandBuffer CommandManager::getCommandBuffer(uint32_t index) const
{
    if (index >= commandBuffers.size()) {
        throw std::out_of_range("Index out of range in getCommandBuffer.");
    }
    return commandBuffers[index];
}
