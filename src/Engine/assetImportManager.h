#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

#include "vulkanUtils.h"

enum class AssetType { Texture, Blender };

class AssetImportManager {
public:
  AssetImportManager();
  ~AssetImportManager();

  // Loads an asset from file based on type.
  // Returns void* which could be std::vector<Vertex>* or other types.
  // The caller must cast appropriately.
  void *importAsset(const std::string &path, AssetType type);

private:
  std::vector<Vertex> importModel(const std::string &path);
  void importTexture(const std::string &path); // Placeholder for now
  void processNode(aiNode *node, const aiScene *scene,
                   std::vector<Vertex> &vertices);
  void processMesh(aiMesh *mesh, const aiScene *scene,
                   std::vector<Vertex> &vertices);
};
