#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "material.h"
#include "mesh.h"
#include "meshManager.h"
#include "vulkanUtils.h"

class DrawManager {
public:
  DrawManager(MeshManager &meshManager);
  ~DrawManager();

  // Draw primitives - returns pointer to created mesh for further manipulation
  Mesh *drawCube(const glm::vec3 &position = glm::vec3(0.0f),
                 const glm::vec3 &scale = glm::vec3(1.0f));

  Mesh *drawPlane(const glm::vec3 &position = glm::vec3(0.0f),
                  float size = 5.0f);

  Mesh *drawTriangle(const glm::vec3 &position = glm::vec3(0.0f));

  Mesh *drawSphere(const glm::vec3 &position = glm::vec3(0.0f),
                   float radius = 1.0f, int sectors = 36, int stacks = 18);

  Mesh *drawCapsule(const glm::vec3 &position = glm::vec3(0.0f),
                    float radius = 0.5f, float height = 2.0f, int sectors = 36,
                    int stacks = 18);

  // Set material on last created mesh
  void setMaterial(Mesh *mesh, std::shared_ptr<Material> material);

private:
  MeshManager &meshManager;

  // Primitive vertex generators
  std::vector<Vertex> generateCubeVertices(const glm::vec3 &position,
                                           const glm::vec3 &scale);
  std::vector<Vertex> generatePlaneVertices(const glm::vec3 &position,
                                            float size);
  std::vector<Vertex> generateTriangleVertices(const glm::vec3 &position);
  std::vector<Vertex> generateSphereVertices(const glm::vec3 &position,
                                             float radius, int sectors,
                                             int stacks);
  std::vector<Vertex> generateCapsuleVertices(const glm::vec3 &position,
                                              float radius, float height,
                                              int sectors, int stacks);
};
