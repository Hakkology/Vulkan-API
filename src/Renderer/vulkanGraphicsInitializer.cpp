#include "vulkanGraphicsInitializer.h"

GraphicsInitializer::GraphicsInitializer(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice)
: instance(instance), device(device), physicalDevice(physicalDevice)
{

}

GraphicsInitializer::~GraphicsInitializer()
{
}

void GraphicsInitializer::addInitialMeshes(MeshManager& meshManager) {
    // Üçgen gibi başlangıç mesh'leri burada eklenir
    std::vector<Vertex> triangleVertices = {
        {{1.0f, 0.0f, 0.0f}},  // Vertex 1
        {{0.0f, 1.0f, 0.0f}},   // Vertex 2
        {{0.0f, 0.0f, 1.0f}}   // Vertex 3
    };
    meshManager.createMesh(triangleVertices);
}
