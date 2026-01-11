#include "meshDrawer.h"
#include <iostream>

static int debugCount = 0;

MeshDrawer::MeshDrawer(VkDevice device, VkRenderPass renderPass,
                       VkPipeline graphicsPipeline, VkPipeline texturedPipeline,
                       VkPipelineLayout pipelineLayout)
    : device(device), renderPass(renderPass),
      graphicsPipeline(graphicsPipeline), texturedPipeline(texturedPipeline),
      pipelineLayout(pipelineLayout) {}

MeshDrawer::~MeshDrawer() {}

void MeshDrawer::drawMesh(VkCommandBuffer commandBuffer, Mesh *mesh,
                          const PushConstants &pushConstants) {
  if (!mesh || !commandBuffer)
    return;

  VkBuffer vertexBuffers[] = {mesh->getVertexBuffer()};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

  // DEBUG: Print which pipeline is being used
  if (debugCount < 20) {
    bool hasMat = mesh->hasMaterial();
    bool hasTex = mesh->hasTexture();
    std::cout << "[MeshDrawer] hasMaterial=" << hasMat
              << ", hasTexture=" << hasTex;
    if (hasMat) {
      auto mat = mesh->getMaterial();
      std::cout << ", matType=" << (int)mat->getType()
                << ", matHasTex=" << mat->hasTexture();
    }
    std::cout << std::endl;
    debugCount++;
  }

  // Bind appropriate pipeline based on texture availability
  if (mesh->hasTexture()) {
    if (debugCount < 25) {
      std::cout << "[MeshDrawer] -> TEXTURED pipeline" << std::endl;
    }
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      texturedPipeline);
  } else {
    if (debugCount < 25) {
      std::cout << "[MeshDrawer] -> COLORED pipeline" << std::endl;
    }
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      graphicsPipeline);
  }

  // Push Constants
  vkCmdPushConstants(commandBuffer, pipelineLayout,
                     VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                     0, sizeof(PushConstants), &pushConstants);

  // Bind the texture descriptor set if available
  if (mesh->hasTexture()) {
    VkDescriptorSet descriptorSet = mesh->getTextureDescriptorSet();
    if (descriptorSet != VK_NULL_HANDLE) {
      vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
    }
  }

  // Draw the mesh
  vkCmdDraw(commandBuffer, static_cast<uint32_t>(mesh->getVertexCount()), 1, 0,
            0);
}
