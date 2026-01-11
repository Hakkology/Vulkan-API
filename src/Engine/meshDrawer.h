#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "vulkanUtils.h"

class MeshDrawer {
public:
  MeshDrawer(VkDevice device, VkRenderPass renderPass,
             VkPipeline graphicsPipeline, VkPipeline texturedPipeline,
             VkPipeline coloredMovingPipeline, VkPipelineLayout pipelineLayout);
  ~MeshDrawer();

  void drawMesh(VkCommandBuffer commandBuffer, Mesh *mesh,
                const PushConstants &pushConstants,
                VkDescriptorSet globalDescriptorSet);

private:
  VkDevice device;
  VkRenderPass renderPass;
  VkPipeline graphicsPipeline;
  VkPipeline texturedPipeline;
  VkPipeline coloredMovingPipeline;
  VkPipelineLayout pipelineLayout;
};