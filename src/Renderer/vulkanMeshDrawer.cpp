#include "vulkanMeshDrawer.h"

MeshDrawer::MeshDrawer(VkDevice device, VkRenderPass renderPass, VkPipeline graphicsPipeline)
    : device(device), renderPass(renderPass), graphicsPipeline(graphicsPipeline) {
}

MeshDrawer::~MeshDrawer() {
    // Clean up operations can be done here, if necessary
    // Note: Destruction of Vulkan objects like renderPass and graphicsPipeline should be managed outside of this class
}

void MeshDrawer::drawMesh(VkCommandBuffer commandBuffer, Mesh* mesh) {
    if (!mesh || !commandBuffer) return;

    VkBuffer vertexBuffers[] = { mesh->getVertexBuffer() }; // Array of vertex buffers
    VkDeviceSize offsets[] = { 0 }; // Offsets for the vertex buffers

    std::cout << "Binding vertex buffers." << std::endl;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    // Command to bind vertex buffer before drawing with them.

    std::cout << "Binding the graphics pipeline." << std::endl;
    // bind render pass to graphics pipeline.
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    // we might have different bind pipelines bound to different set of functions here.

    // Draw the mesh
    vkCmdDraw(commandBuffer, static_cast<uint32_t>(mesh->getVertexCount()), 1, 0, 0);
}
