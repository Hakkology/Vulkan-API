#pragma once

#include <array>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

class Renderpass {
public:
  Renderpass(VkDevice device, VkFormat swapChainImageFormat,
             VkFormat depthFormat);
  ~Renderpass();

  void createRenderPass();
  void cleanup();

  // Get a reference to the render pass
  VkRenderPass &getRenderPass() { return renderPass; }

private:
  VkDevice device;
  VkRenderPass renderPass;
  VkFormat swapChainImageFormat;
  VkFormat depthFormat;

  // Private methods to setup different parts of the render pass
  std::vector<VkAttachmentDescription> createAttachmentDescriptions();
  std::vector<VkSubpassDescription> createSubpassDescriptions();
  std::vector<VkSubpassDependency> createSubpassDependencies();
};
