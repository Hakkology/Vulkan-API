#include "vulkanRenderpass.h"
#include <iostream>

Renderpass::Renderpass(VkDevice device, VkFormat swapChainImageFormat):
device(device), swapChainImageFormat(swapChainImageFormat) {}

Renderpass::~Renderpass()
{

}

void Renderpass::createRenderPass()
{
    // Colour attachment of render pass.
    VkAttachmentDescription colourAttachment = {};
    colourAttachment.format = swapChainImageFormat;                     // format to use for attachment.
    colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;                   // same as rasterization, number of samples to write for multisampling.
    colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;              // describes what to do with attachment before rendering.
    colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;            // describes what to do with attachment after rendering.
    colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;   // describes what to do with stencil before rendering.
    colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // describes what to do with stencil after rendering.
    // Framebuffer data will be stored as an image, but images can be given different data layouts.
    // to give optimal use for certain operations.
    colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;         // Image data layout before render pass starts
    colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;     // Image data layout after render pass ( to change to)

    // Attachment reference uses an attachment index that refers to index in the attachment list passed to renderPassCreateInfo
    VkAttachmentReference colourAttachmentReference = {};
    colourAttachmentReference.attachment = 0;
    colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Information about a particular subpass the Render pass is using.
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;        // Pipeline type subpass is bound to.
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colourAttachmentReference;

    // Need to determine when layout transitions occur using subpass dependencies
    std::array<VkSubpassDependency, 2> subpassDependencies;

    // Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOUR_ATTACHMENT_OPTIMAL
    // Transition must happen after...
    subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;            // Sub pass index (VK_SUBPASS_EXTERNAL = Special value meaning outside of renderpass.)
    subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;    // Pipeline stage
    subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;   // It has to be read from before the conversion.
    // Transition must happen before...
    subpassDependencies[0].dstSubpass = 0;                              // Amount of subpasses. It is 1 now.
    subpassDependencies[0].dstStageMask = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  
    subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependencies[0].dependencyFlags = 0;
    // Two colour attachment optimal has to happen after the end of external subpass, before the colour output of our main subpass.
    // It also needs to happen before it attempts to read or write to it.
    
    // Conversion from VK_IMAGE_LAYOUT_COLOUR_ATTACHMENT_OPTIMAL to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    // Transition must happen after...
    subpassDependencies[1].srcSubpass = 0;                              // Sub pass index (VK_SUBPASS_EXTERNAL = Special value meaning outside of renderpass.)
    subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;    // Pipeline stage
    subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;  
    // Transition must happen before...
    subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                                // Amount of subpasses. It is 1 now.
    subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; 
    subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    subpassDependencies[1].dependencyFlags = 0;

    // Create info for render pass
    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &colourAttachment;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
    renderPassCreateInfo.pDependencies = subpassDependencies.data();

    VkResult result = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);
    if (result!= VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create a render pass.");
    }
    
    std::cout << "Render pass initialized successfully..." << std::endl;
}

void Renderpass::cleanup()
{
    if (renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(device, renderPass, nullptr);
        renderPass = VK_NULL_HANDLE;
        std::cout << "Render pass destroyed." << std::endl;
    }
}
