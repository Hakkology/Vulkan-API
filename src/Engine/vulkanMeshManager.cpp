#include "vulkanMeshManager.h"

MeshManager::MeshManager(VkPhysicalDevice physicalDevice, VkDevice device)
    : physicalDevice(physicalDevice), device(device) {}

MeshManager::~MeshManager() { meshes.clear(); }

Mesh *MeshManager::createMesh(const std::vector<Vertex> &vertices) {
  // Create a new mesh object
  std::vector<Vertex> *nonConstVertices =
      const_cast<std::vector<Vertex> *>(&vertices);

  // Yeni mesh nesnesini oluştur
  auto mesh = std::make_unique<Mesh>(physicalDevice, device, nonConstVertices);
  int meshId = nextMeshId++;
  meshes[meshId] = std::move(mesh);
  return meshes[meshId].get();
}

void MeshManager::destroyMesh(Mesh *mesh) {
  // Find the mesh by searching for its pointer in the map
  for (auto it = meshes.begin(); it != meshes.end(); ++it) {
    if (it->second.get() == mesh) {
      // Destroy the mesh explicitly if needed
      it->second->destroyVertexBuffer();
      meshes.erase(it);
      break;
    }
  }
}

Mesh *MeshManager::getMeshById(int id) const {
  auto it = meshes.find(id);
  if (it != meshes.end()) {
    return it->second.get();
  }
  return nullptr;
}

const std::unordered_map<int, std::unique_ptr<Mesh>> &
MeshManager::getAllMeshes() const {
  return meshes;
}