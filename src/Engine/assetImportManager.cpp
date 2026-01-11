#include "assetImportManager.h"
#include <iostream>

AssetImportManager::AssetImportManager() {}

AssetImportManager::~AssetImportManager() {}

void *AssetImportManager::importAsset(const std::string &path, AssetType type) {
  if (type == AssetType::Blender) {
    std::vector<Vertex> *vertices = new std::vector<Vertex>();
    *vertices = importModel(path);
    if (vertices->empty()) {
      delete vertices;
      return nullptr;
    }
    return vertices;
  } else if (type == AssetType::Texture) {
    importTexture(path);
    return nullptr;
  }
  return nullptr;
}

void AssetImportManager::importTexture(const std::string &path) {
  std::cout << "AssetImportManager: Texture import requested for " << path
            << ". (Placeholder - Logic currently in TextureManager)"
            << std::endl;
}

std::vector<Vertex> AssetImportManager::importModel(const std::string &path) {
  Assimp::Importer importer;
  // aiProcess_Triangulate: Triangulates all faces
  // aiProcess_FlipUVs: Flips texture coordinates along y-axis
  // aiProcess_GenNormals: Generates normals if they are missing
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return {};
  }

  std::vector<Vertex> vertices;
  processNode(scene->mRootNode, scene, vertices);
  return vertices;
}

void AssetImportManager::processNode(aiNode *node, const aiScene *scene,
                                     std::vector<Vertex> &vertices) {
  // Process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(mesh, scene, vertices);
  }
  // Then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, vertices);
  }
}

void AssetImportManager::processMesh(aiMesh *mesh, const aiScene *scene,
                                     std::vector<Vertex> &vertices) {
  // Iterate over faces instead of just vertices to handle indexed geometry.
  // We expand the mesh into a triangle list (vertex soup) compatible with
  // non-indexed drawing.
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      unsigned int index = face.mIndices[j];

      Vertex vertex;

      // Positions
      vertex.pos.x = mesh->mVertices[index].x;
      vertex.pos.y = mesh->mVertices[index].y;
      vertex.pos.z = mesh->mVertices[index].z;

      // Normals
      if (mesh->HasNormals()) {
        vertex.normal.x = mesh->mNormals[index].x;
        vertex.normal.y = mesh->mNormals[index].y;
        vertex.normal.z = mesh->mNormals[index].z;
      } else {
        vertex.normal = glm::vec3(0.0f);
      }

      // Texture Coordinates
      if (mesh->mTextureCoords[0]) {
        vertex.tex.x = mesh->mTextureCoords[0][index].x;
        vertex.tex.y = mesh->mTextureCoords[0][index].y;
      } else {
        vertex.tex = glm::vec2(0.0f, 0.0f);
      }

      vertices.push_back(vertex);
    }
  }
}
