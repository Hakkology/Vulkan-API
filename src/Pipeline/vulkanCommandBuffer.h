#pragma once

#include <swapChainManager.h>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkanUtils.h>

#include "vulkanMeshDrawer.h"
#include "vulkanMeshHandler.h"
#include "vulkanMeshManager.h"

class CommandManager {
public:
  CommandManager(VkDevice device, VkPhysicalDevice physicalDevice,
                 VkSurfaceKHR surface, VkPipeline graphicsPipeline);
  ~CommandManager();

  void createCommandPool();
  void allocateCommandBuffers(std::vector<VkFramebuffer> frameBuffers);
  void recordCommands(std::vector<VkFramebuffer> frameBuffers,
                      VkRenderPass &renderPass, VkExtent2D chosenExtent,
                      MeshDrawer *meshDrawer, MeshManager *meshManager);
  void recordCommand(uint32_t imageIndex, VkFramebuffer framebuffer,
                     VkRenderPass &renderPass, VkExtent2D chosenExtent,
                     MeshDrawer *meshDrawer, MeshManager *meshManager,
                     const PushConstants &pushConstants);
  void cleanup();

  // Getters
  VkCommandBuffer getCommandBuffer(uint32_t index) const;
  VkCommandPool getCommandPool() const { return graphicsCommandPool; }

private:
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkSurfaceKHR &surface;
  VkPipeline graphicsPipeline;

  VkCommandPool graphicsCommandPool;
  // std::vector<VkFramebuffer>& frameBuffers;
  std::vector<VkCommandBuffer> commandBuffers;
};
