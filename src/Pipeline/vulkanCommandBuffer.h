#pragma once

#include <array>
#include <swapChainManager.h>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkanUtils.h>

#include "mesh.h"
#include "meshDrawer.h"
#include "meshManager.h"

class CommandManager {
public:
  CommandManager(VkDevice device, VkPhysicalDevice physicalDevice,
                 VkSurfaceKHR surface, VkPipeline graphicsPipeline);
  ~CommandManager();

  void createCommandPool();
  void allocateCommandBuffers(std::vector<VkFramebuffer> frameBuffers);
  void recordCommands(std::vector<VkFramebuffer> frameBuffers,
                      VkRenderPass &renderPass, VkExtent2D chosenExtent,
                      MeshDrawer *meshDrawer, MeshManager *meshManager,
                      VkDescriptorSet globalDescriptorSet);
  void recordMainPass(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer,
                      VkRenderPass &renderPass, VkExtent2D chosenExtent,
                      MeshDrawer *meshDrawer, MeshManager *meshManager,
                      const PushConstants &pushConstants,
                      VkDescriptorSet globalDescriptorSet);
  void recordShadowPass(VkCommandBuffer commandBuffer, MeshManager *meshManager,
                        VkPipeline shadowPipeline,
                        VkPipelineLayout pipelineLayout,
                        VkExtent2D shadowExtent, VkRenderPass shadowRenderPass,
                        VkFramebuffer shadowFramebuffer,
                        const glm::mat4 &lightSpaceMatrix);
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
