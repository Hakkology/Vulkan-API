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

Mesh *DrawManager::drawSphere(const glm::vec3 &position, float radius,
                              int sectors, int stacks) {
  auto vertices = generateSphereVertices(position, radius, sectors, stacks);
  Mesh *mesh = meshManager.createMesh(vertices);
  return mesh;
}

Mesh *DrawManager::drawCapsule(const glm::vec3 &position, float radius,
                               float height, int sectors, int stacks) {
  auto vertices =
      generateCapsuleVertices(position, radius, height, sectors, stacks);
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

std::vector<Vertex>
DrawManager::generateSphereVertices(const glm::vec3 &position, float radius,
                                    int sectors, int stacks) {
  std::vector<Vertex> vertices;
  float x, y, z, xy;                           // vertex position
  float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
  float s, t;                                  // vertex texCoord

  float sectorStep = 2 * M_PI / sectors;
  float stackStep = M_PI / stacks;
  float sectorAngle, stackAngle;

  // Vertices
  for (int i = 0; i <= stacks; ++i) {
    stackAngle = M_PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
    xy = radius * cosf(stackAngle);        // r * cos(u)
    z = radius * sinf(stackAngle);         // r * sin(u)

    // add (sectorCount+1) vertices per stack
    // the first and last vertices have same position and normal, but different
    // tex coords
    for (int j = 0; j <= sectors; ++j) {
      sectorAngle = j * sectorStep; // starting from 0 to 2pi

      // vertex position (x, y, z)
      x = xy * cosf(sectorAngle);
      y = xy * sinf(sectorAngle);

      // Calculate Y-up spherical coordinates
      float py = radius * sinf(stackAngle);
      float pxz = radius * cosf(stackAngle);
      float px = pxz * cosf(sectorAngle);
      float pz = pxz * sinf(sectorAngle);

      glm::vec3 pos = glm::vec3(px, py, pz);
      glm::vec3 normal = glm::normalize(pos);

      // Tex coords
      s = (float)j / sectors;
      t = (float)i / stacks;

      vertices.push_back({position + pos, normal, {s, t}});
    }
  }

  // Triangulate sphere by duplicating vertices (non-indexed drawing)
  std::vector<Vertex> triangleVertices;
  int k1, k2;
  for (int i = 0; i < stacks; ++i) {
    k1 = i * (sectors + 1); // beginning of current stack
    k2 = k1 + sectors + 1;  // beginning of next stack

    for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0) {
        triangleVertices.push_back(vertices[k1]);
        triangleVertices.push_back(vertices[k2]);
        triangleVertices.push_back(vertices[k1 + 1]);
      }

      // k1+1 => k2 => k2+1
      if (i != (stacks - 1)) {
        triangleVertices.push_back(vertices[k1 + 1]);
        triangleVertices.push_back(vertices[k2]);
        triangleVertices.push_back(vertices[k2 + 1]);
      }
    }
  }
  return triangleVertices;
}

std::vector<Vertex>
DrawManager::generateCapsuleVertices(const glm::vec3 &position, float radius,
                                     float height, int sectors, int stacks) {
  std::vector<Vertex> generatedVertices;
  std::vector<Vertex> gridVertices;

  // Total steps = stacks (top hemi) + 1 (cylinder) + stacks (bottom hemi).
  // 0 to stacks: Top Hemisphere
  // stacks to stacks+1: Cylinder
  // stacks+1 to 2*stacks+1: Bottom Hemisphere

  for (int i = 0; i <= (2 * stacks + 1); ++i) {
    float y, xz;

    if (i <= stacks) {
      // Top Hemisphere
      float ratio = (float)i / stacks;
      float angle = M_PI / 2 * (1.0f - ratio);
      y = radius * sinf(angle) + height * 0.5f;
      xz = radius * cosf(angle);
    } else if (i == stacks + 1) {
      // Bottom of cylinder
      y = -height * 0.5f;
      xz = radius;
    } else {
      // Bottom Hemisphere
      float ratio = (float)(i - (stacks + 1)) / stacks;
      float angle = -M_PI / 2 * ratio;
      y = radius * sinf(angle) - height * 0.5f;
      xz = radius * cosf(angle);
    }

    for (int j = 0; j <= sectors; ++j) {
      float theta = (float)j / sectors * 2 * M_PI;
      float x = xz * cosf(theta);
      float z = xz * sinf(theta);

      // Normal calculation
      glm::vec3 n;
      if (y > height * 0.5f - 0.001f) {
        n = glm::normalize(glm::vec3(x, y - height * 0.5f, z));
      } else if (y < -height * 0.5f + 0.001f) {
        n = glm::normalize(glm::vec3(x, y + height * 0.5f, z));
      } else {
        n = glm::normalize(glm::vec3(x, 0, z));
      }

      gridVertices.push_back(
          {position + glm::vec3(x, y, z),
           n,
           {(float)j / sectors, (float)i / (2 * stacks + 1)}});
    }
  }

  // Triangulate grid
  int numRows = 2 * stacks + 1;
  for (int i = 0; i < numRows; ++i) {
    int k1 = i * (sectors + 1);
    int k2 = k1 + sectors + 1;

    for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
      generatedVertices.push_back(gridVertices[k1]);
      generatedVertices.push_back(gridVertices[k2]);
      generatedVertices.push_back(gridVertices[k1 + 1]);

      generatedVertices.push_back(gridVertices[k1 + 1]);
      generatedVertices.push_back(gridVertices[k2]);
      generatedVertices.push_back(gridVertices[k2 + 1]);
    }
  }

  return generatedVertices;
}
