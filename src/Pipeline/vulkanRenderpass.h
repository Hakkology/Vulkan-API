#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <stdexcept>


class Renderpass {
public:
    Renderpass(VkDevice device, VkFormat swapChainImageFormat);
    ~Renderpass();

    void createRenderPass();
    void cleanup();

    // Get a reference to the render pass
    VkRenderPass getRenderPass() const { return renderPass; }

private:
    VkDevice device;
    VkRenderPass renderPass;
    VkFormat swapChainImageFormat;

    // Private methods to setup different parts of the render pass
    std::vector<VkAttachmentDescription> createAttachmentDescriptions();
    std::vector<VkSubpassDescription> createSubpassDescriptions();
    std::vector<VkSubpassDependency> createSubpassDependencies();

    VkResult initializeRenderPass();
};
