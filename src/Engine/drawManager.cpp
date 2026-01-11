#include "drawManager.h"

DrawManager::DrawManager(MeshManager &meshManager) : meshManager(meshManager) {}

DrawManager::~DrawManager() {}

Mesh *DrawManager::drawCube(const glm::vec3 &position, const glm::vec3 &scale) {
  auto vertices = generateCubeVertices(position, scale);
  Mesh *mesh = meshManager.createMesh(vertices);
  return mesh;
}

Mesh *DrawManager::drawPlane(const glm::vec3 &position, float size) {
  auto vertices = generatePlaneVertices(position, size);
  Mesh *mesh = meshManager.createMesh(vertices);
  return mesh;
}

Mesh *DrawManager::drawTriangle(const glm::vec3 &position) {
  auto vertices = generateTriangleVertices(position);
  Mesh *mesh = meshManager.createMesh(vertices);
  return mesh;
}

void DrawManager::setMaterial(Mesh *mesh, std::shared_ptr<Material> material) {
  if (mesh) {
    mesh->setMaterial(material);
  }
}

std::vector<Vertex> DrawManager::generateCubeVertices(const glm::vec3 &position,
                                                      const glm::vec3 &scale) {
  float sx = scale.x * 0.5f;
  float sy = scale.y * 0.5f;
  float sz = scale.z * 0.5f;
  float px = position.x;
  float py = position.y;
  float pz = position.z;

  return {// Front face
          {{px - sx, py - sy, pz + sz}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
          {{px + sx, py - sy, pz + sz}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
          {{px + sx, py + sy, pz + sz}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
          {{px - sx, py - sy, pz + sz}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
          {{px + sx, py + sy, pz + sz}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
          {{px - sx, py + sy, pz + sz}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

          // Back face
          {{px + sx, py - sy, pz - sz}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
          {{px - sx, py - sy, pz - sz}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
          {{px - sx, py + sy, pz - sz}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
          {{px + sx, py - sy, pz - sz}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
          {{px - sx, py + sy, pz - sz}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
          {{px + sx, py + sy, pz - sz}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

          // Left face
          {{px - sx, py - sy, pz - sz}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
          {{px - sx, py - sy, pz + sz}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
          {{px - sx, py + sy, pz + sz}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - sx, py - sy, pz - sz}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
          {{px - sx, py + sy, pz + sz}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - sx, py + sy, pz - sz}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

          // Right face
          {{px + sx, py - sy, pz + sz}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + sx, py - sy, pz - sz}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
          {{px + sx, py + sy, pz - sz}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
          {{px + sx, py - sy, pz + sz}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + sx, py + sy, pz - sz}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
          {{px + sx, py + sy, pz + sz}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

          // Top face
          {{px - sx, py + sy, pz + sz}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + sx, py + sy, pz + sz}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
          {{px + sx, py + sy, pz - sz}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - sx, py + sy, pz + sz}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + sx, py + sy, pz - sz}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - sx, py + sy, pz - sz}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

          // Bottom face
          {{px - sx, py - sy, pz - sz}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + sx, py - sy, pz - sz}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
          {{px + sx, py - sy, pz + sz}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - sx, py - sy, pz - sz}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + sx, py - sy, pz + sz}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - sx, py - sy, pz + sz}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}};
}

std::vector<Vertex>
DrawManager::generatePlaneVertices(const glm::vec3 &position, float size) {
  float y = position.y;
  float px = position.x;
  float pz = position.z;
  float s = size;

  return {{{px - s, y, pz + s}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + s, y, pz + s}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
          {{px + s, y, pz - s}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - s, y, pz + s}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + s, y, pz - s}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
          {{px - s, y, pz - s}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};
}

std::vector<Vertex>
DrawManager::generateTriangleVertices(const glm::vec3 &position) {
  float px = position.x;
  float py = position.y;
  float pz = position.z;

  return {{{px + 0.0f, py - 0.5f, pz}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
          {{px + 0.5f, py + 0.5f, pz}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
          {{px - 0.5f, py + 0.5f, pz}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}};
}
