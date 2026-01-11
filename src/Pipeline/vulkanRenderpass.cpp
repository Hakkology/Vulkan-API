#include "vulkanRenderpass.h"
#include <iostream>

Renderpass::Renderpass(VkDevice device, VkFormat swapChainImageFormat,
                       VkFormat depthFormat)
    : device(device), swapChainImageFormat(swapChainImageFormat),
      depthFormat(depthFormat) {}

Renderpass::~Renderpass() {}

void Renderpass::createRenderPass() {
  // Colour attachment of render pass.
  VkAttachmentDescription colourAttachment = {};
  colourAttachment.format =
      swapChainImageFormat; // format to use for attachment.
  colourAttachment.samples =
      VK_SAMPLE_COUNT_1_BIT; // same as rasterization, number of samples to
                             // write for multisampling.
  colourAttachment.loadOp =
      VK_ATTACHMENT_LOAD_OP_CLEAR; // describes what to do with attachment
                                   // before rendering.
  colourAttachment.storeOp =
      VK_ATTACHMENT_STORE_OP_STORE; // describes what to do with attachment
                                    // after rendering.
  colourAttachment.stencilLoadOp =
      VK_ATTACHMENT_LOAD_OP_DONT_CARE; // describes what to do with stencil
                                       // before rendering.
  colourAttachment.stencilStoreOp =
      VK_ATTACHMENT_STORE_OP_DONT_CARE; // describes what to do with stencil
                                        // after rendering.
  // Framebuffer data will be stored as an image, but images can be given
  // different data layouts. to give optimal use for certain operations.
  colourAttachment.initialLayout =
      VK_IMAGE_LAYOUT_UNDEFINED; // Image data layout before render pass starts
  colourAttachment.finalLayout =
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // Image data layout after render pass
                                       // (to change to)

  // Attachment reference uses an attachment index that refers to index in the
  // attachment list passed to renderPassCreateInfo
  VkAttachmentReference colourAttachmentReference = {};
  colourAttachmentReference.attachment = 0;
  colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // Depth attachment
  VkAttachmentDescription depthAttachment = {};
  depthAttachment.format = depthFormat; // Use member variable
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentReference = {};
  depthAttachmentReference.attachment = 1;
  depthAttachmentReference.layout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colourAttachmentReference;
  subpass.pDepthStencilAttachment = &depthAttachmentReference;

  std::array<VkSubpassDependency, 2> subpassDependencies;

  // Conversion from VK_IMAGE_LAYOUT_UNDEFINED to
  // VK_IMAGE_LAYOUT_COLOUR_ATTACHMENT_OPTIMAL
  subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  subpassDependencies[0].dstSubpass = 0;
  subpassDependencies[0].dstStageMask =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  subpassDependencies[0].dependencyFlags = 0;

  // Conversion from VK_IMAGE_LAYOUT_COLOUR_ATTACHMENT_OPTIMAL to
  // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
  subpassDependencies[1].srcSubpass = 0;
  subpassDependencies[1].srcStageMask =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
  subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  subpassDependencies[1].dependencyFlags = 0;

  std::array<VkAttachmentDescription, 2> attachments = {colourAttachment,
                                                        depthAttachment};

  VkRenderPassCreateInfo renderPassCreateInfo = {};
  renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassCreateInfo.attachmentCount =
      static_cast<uint32_t>(attachments.size());
  renderPassCreateInfo.pAttachments = attachments.data();
  renderPassCreateInfo.subpassCount = 1;
  renderPassCreateInfo.pSubpasses = &subpass;
  renderPassCreateInfo.dependencyCount =
      static_cast<uint32_t>(subpassDependencies.size());
  renderPassCreateInfo.pDependencies = subpassDependencies.data();

  VkResult result =
      vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to create a render pass.");
  }

  std::cout << "Render pass initialized successfully..." << std::endl;
}
void Renderpass::cleanup() {
  if (renderPass != VK_NULL_HANDLE) {
    vkDestroyRenderPass(device, renderPass, nullptr);
    renderPass = VK_NULL_HANDLE;
    std::cout << "Render pass destroyed." << std::endl;
  }
}
