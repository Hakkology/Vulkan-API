#include "meshDrawer.h"

MeshDrawer::MeshDrawer(VkDevice device, VkRenderPass renderPass,
                       VkPipeline graphicsPipeline, VkPipeline texturedPipeline,
                       VkPipeline coloredMovingPipeline,
                       VkPipelineLayout pipelineLayout)
    : device(device), renderPass(renderPass),
      graphicsPipeline(graphicsPipeline), texturedPipeline(texturedPipeline),
      coloredMovingPipeline(coloredMovingPipeline),
      pipelineLayout(pipelineLayout) {}

MeshDrawer::~MeshDrawer() {}

void MeshDrawer::drawMesh(VkCommandBuffer commandBuffer, Mesh *mesh,
                          const PushConstants &pushConstants,
                          VkDescriptorSet globalDescriptorSet) {
  if (!mesh || !commandBuffer)
    return;

  VkBuffer vertexBuffers[] = {mesh->getVertexBuffer()};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

  // Bind appropriate pipeline based on texture availability
  // Bind appropriate pipeline based on material type
  VkPipeline pipelineToBind = graphicsPipeline; // Default

  if (mesh->hasMaterial()) {
    MaterialType type = mesh->getMaterial()->getType();
    if (type == MaterialType::COLORED_MOVING) {
      pipelineToBind = coloredMovingPipeline;
    } else if (type == MaterialType::TEXTURED) {
      pipelineToBind = texturedPipeline;
    }
  }

  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipelineToBind);

  // Bind Global Descriptor Set (Set 0)
  if (globalDescriptorSet != VK_NULL_HANDLE) {
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pipelineLayout, 0, 1, &globalDescriptorSet, 0,
                            nullptr);
  }

  // Bind the texture descriptor set if available (Set 1)
  if (mesh->hasTexture()) {
    VkDescriptorSet descriptorSet = mesh->getTextureDescriptorSet();
    if (descriptorSet != VK_NULL_HANDLE) {
      vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              pipelineLayout, 1, 1, &descriptorSet, 0, nullptr);
    }
  }

  // Push Constants
  vkCmdPushConstants(commandBuffer, pipelineLayout,
                     VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                     0, sizeof(PushConstants), &pushConstants);

  // Draw the mesh
  vkCmdDraw(commandBuffer, static_cast<uint32_t>(mesh->getVertexCount()), 1, 0,
            0);
}
