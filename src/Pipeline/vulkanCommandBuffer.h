#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <vulkanUtils.h>
#include <swapChainManager.h>

class CommandManager {
public:
    CommandManager(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkPipeline graphicsPipeline);
    ~CommandManager();

    void createCommandPool();
    void allocateCommandBuffers(std::vector<VkFramebuffer> frameBuffers);
    void recordCommands(std::vector<VkFramebuffer> frameBuffers, VkRenderPass& renderpass, VkExtent2D chosenExtent);
    void cleanup();

    // Getters
    VkCommandBuffer getCommandBuffer(uint32_t index) const;
    VkCommandPool getCommandPool() const;

private:
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR& surface;
    VkPipeline graphicsPipeline;

    VkCommandPool graphicsCommandPool;
    //std::vector<VkFramebuffer>& frameBuffers;
    std::vector<VkCommandBuffer> commandBuffers;

};
