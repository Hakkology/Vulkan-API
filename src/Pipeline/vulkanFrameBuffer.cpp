#include "vulkanFrameBuffer.h"

FrameManager::FrameManager(VkDevice device, SwapChainManager& swapchainManager, VkRenderPass renderpass)
: device(device), swapchainManager(swapchainManager), renderpass(renderpass) {}

FrameManager::~FrameManager() {}

void FrameManager::createFrameBuffers()
{
    // resize framebuffer count to equal swapchain image count.
    std::vector<SwapchainImage> swapchainImages = swapchainManager.getSwapChainImages();
    swapchainFrameBuffers.resize(swapchainImages.size());

    // create a framebuffer for each swapchain image
    for (size_t i = 0; i < swapchainFrameBuffers.size(); i++)
    {

        std::array<VkImageView, 1> attachments = {
            swapchainImages[i].imageView
            // these need to be ordered in the same way as colour attachments in render pass.
        };

        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = renderpass;                                      // Render pass layout the framebuffer will be used with.
        framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());  
        framebufferCreateInfo.pAttachments = attachments.data();                            // list of attachments (1:1 with render pass).
        framebufferCreateInfo.width = swapchainManager.getChosenExtent().width;             // framebuffer width.
        framebufferCreateInfo.height = swapchainManager.getChosenExtent().height;           // framebuffer height.
        framebufferCreateInfo.layers = 1;                                                   // framebuffer layers.

        VkResult result = vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &swapchainFrameBuffers[i]);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create a Framebuffer");
        }

        std::cout << "Frame buffers configured successfully." << std::endl;
    }
}

void FrameManager::cleanup()
{
    // Loop through all the framebuffers and destroy each one
    for (auto& framebuffer : swapchainFrameBuffers) {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
            framebuffer = VK_NULL_HANDLE; // Explicitly set to VK_NULL_HANDLE after destruction
        }
    }

    // Clear the vector of framebuffers after all have been destroyed
    swapchainFrameBuffers.clear();
}
