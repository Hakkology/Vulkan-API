#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkanMeshHandler.h"
#include "vulkanUtils.h"

class MeshDrawer {
public:
    MeshDrawer(VkDevice device, VkRenderPass renderPass, VkPipeline graphicsPipeline);
    ~MeshDrawer();

    void drawMesh(VkCommandBuffer commandBuffer, Mesh* mesh);

private:
    VkDevice device;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
};