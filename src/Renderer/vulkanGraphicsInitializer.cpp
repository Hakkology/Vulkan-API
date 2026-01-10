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
    // Cube vertices (36 vertices for 12 triangles) with Normals
    vertices = {// Front face
                {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

                // Back face
                {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
                {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},

                // Left face
                {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},

                // Right face
                {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},

                // Top face
                {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},

                // Bottom face
                {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
                {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
                {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
                {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
                {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
                {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}}};
  };

  if (!vertices.empty()) {
    meshManager.createMesh(vertices);
  }
}

void GraphicsInitializer::initLights(LightManager &lightManager) {
  // Sahnede asimetrik bir directional light olsun (Yüzeyleri farklı aydınlatsın
  // diye)
  glm::vec3 direction(5.0f, 10.0f, 2.0f);
  glm::vec3 color(1.0f, 1.0f, 1.0f);
  lightManager.setDirectionalLight(direction, color);
}
