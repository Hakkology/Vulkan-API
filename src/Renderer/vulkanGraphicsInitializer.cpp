#include "vulkanGraphicsInitializer.h"

GraphicsInitializer::GraphicsInitializer(VkInstance instance, VkDevice device,
                                         VkPhysicalDevice physicalDevice)
    : instance(instance), device(device), physicalDevice(physicalDevice) {}

GraphicsInitializer::~GraphicsInitializer() {}

void GraphicsInitializer::addInitialMeshes(MeshManager &meshManager,
                                           ShapeType shapeType) {
  std::vector<Vertex> vertices;

  if (shapeType == ShapeType::TRIANGLE) {
    // Üçgen gibi başlangıç mesh'leri burada eklenir
    vertices = {
        {{1.0f, 0.0f, 0.0f}}, // Vertex 1
        {{0.0f, 1.0f, 0.0f}}, // Vertex 2
        {{0.0f, 0.0f, 1.0f}}  // Vertex 3
    };
  } else if (shapeType == ShapeType::CUBE) {
    // Cube vertices (36 vertices for 12 triangles)
    vertices = {// Front face
                {{-0.5f, -0.5f, 0.5f}},
                {{0.5f, -0.5f, 0.5f}},
                {{0.5f, 0.5f, 0.5f}},
                {{-0.5f, -0.5f, 0.5f}},
                {{0.5f, 0.5f, 0.5f}},
                {{-0.5f, 0.5f, 0.5f}},

                // Back face
                {{0.5f, -0.5f, -0.5f}},
                {{-0.5f, -0.5f, -0.5f}},
                {{-0.5f, 0.5f, -0.5f}},
                {{0.5f, -0.5f, -0.5f}},
                {{-0.5f, 0.5f, -0.5f}},
                {{0.5f, 0.5f, -0.5f}},

                // Left face
                {{-0.5f, -0.5f, -0.5f}},
                {{-0.5f, -0.5f, 0.5f}},
                {{-0.5f, 0.5f, 0.5f}},
                {{-0.5f, -0.5f, -0.5f}},
                {{-0.5f, 0.5f, 0.5f}},
                {{-0.5f, 0.5f, -0.5f}},

                // Right face
                {{0.5f, -0.5f, 0.5f}},
                {{0.5f, -0.5f, -0.5f}},
                {{0.5f, 0.5f, -0.5f}},
                {{0.5f, -0.5f, 0.5f}},
                {{0.5f, 0.5f, -0.5f}},
                {{0.5f, 0.5f, 0.5f}},

                // Top face
                {{-0.5f, 0.5f, 0.5f}},
                {{0.5f, 0.5f, 0.5f}},
                {{0.5f, 0.5f, -0.5f}},
                {{-0.5f, 0.5f, 0.5f}},
                {{0.5f, 0.5f, -0.5f}},
                {{-0.5f, 0.5f, -0.5f}},

                // Bottom face
                {{-0.5f, -0.5f, -0.5f}},
                {{0.5f, -0.5f, -0.5f}},
                {{0.5f, -0.5f, 0.5f}},
                {{-0.5f, -0.5f, -0.5f}},
                {{0.5f, -0.5f, 0.5f}},
                {{-0.5f, -0.5f, 0.5f}}};
  }

  if (!vertices.empty()) {
    meshManager.createMesh(vertices);
  }
}
