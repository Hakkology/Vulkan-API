#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

#include "vulkanUtils.h"

class AssetImportManager {
public:
  AssetImportManager();
  ~AssetImportManager();

  // Loads a model from file and returns a vector of vertices.
  // In the future, this can return a more complex structure (e.g., Model object
  // with multiple meshes and materials).
  std::vector<Vertex> loadModel(const std::string &path);

private:
  void processNode(aiNode *node, const aiScene *scene,
                   std::vector<Vertex> &vertices);
  void processMesh(aiMesh *mesh, const aiScene *scene,
                   std::vector<Vertex> &vertices);
};
