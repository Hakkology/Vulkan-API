#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "mesh.h"
#include "vulkanUtils.h"

class MeshManager {
public:
  MeshManager(VkPhysicalDevice physicalDevice, VkDevice device);
  ~MeshManager();

  Mesh *createMesh(const std::vector<Vertex> &vertices);
  void destroyMesh(Mesh *mesh);
  Mesh *getMeshById(int id) const;
  const std::unordered_map<int, std::unique_ptr<Mesh>> &getAllMeshes() const;

private:
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  std::unordered_map<int, std::unique_ptr<Mesh>> meshes;
  int nextMeshId = 0;
};
