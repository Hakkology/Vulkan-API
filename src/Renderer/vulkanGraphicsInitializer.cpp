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
  } else if (shapeType == ShapeType::PLANE) {
    // Plane vertices (6 vertices for 2 triangles)
    // Sits on XZ plane, y = -1.0 (below cube with gap)
    float y = -1.0f;
    float s = 5.0f; // Size
    vertices = {
        {{-s, y, s}, {0.0f, 1.0f, 0.0f}}, {{s, y, s}, {0.0f, 1.0f, 0.0f}},
        {{s, y, -s}, {0.0f, 1.0f, 0.0f}}, {{-s, y, s}, {0.0f, 1.0f, 0.0f}},
        {{s, y, -s}, {0.0f, 1.0f, 0.0f}}, {{-s, y, -s}, {0.0f, 1.0f, 0.0f}}};
  }

  if (!vertices.empty()) {
    Mesh *newMesh = meshManager.createMesh(vertices);
    if (newMesh) {
      if (shapeType == ShapeType::CUBE) {
        newMesh->setColor(glm::vec4(1.0f, 0.5f, 0.31f, 1.0f)); // Orange
      } else if (shapeType == ShapeType::PLANE) {
        newMesh->setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // Grey
      }
    }
  }
}

void GraphicsInitializer::initLights(LightManager &lightManager) {
  // Sahnede asimetrik bir directional light olsun (Yüzeyleri farklı aydınlatsın
  // diye)
  // Light coming from top-right-front, slightly angled to hit 3 faces
  // differently Vector TO Light approx (2, 4, 1). Top (~0,1,0) dot (2,4,1) ~ 4
  // (Strongest) Right (~1,0,0) dot (2,4,1) ~ 2 (Medium) Front (~0,0,1) dot
  // (2,4,1) ~ 1 (Weak) Direction of rays = - (Vector TO Light)
  glm::vec3 direction(-2.0f, -4.0f, -1.0f);
  glm::vec3 color(1.0f, 1.0f, 1.0f);
  lightManager.setDirectionalLight(direction, color);

  // Add stronger ambient light to make shadows visible/not pitch black
  lightManager.setAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.4f);
}
