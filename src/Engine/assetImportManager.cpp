#include "assetImportManager.h"
#include <iostream>

AssetImportManager::AssetImportManager() {}

AssetImportManager::~AssetImportManager() {}

std::vector<Vertex> AssetImportManager::loadModel(const std::string &path) {
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
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    // Positions
    vertex.pos.x = mesh->mVertices[i].x;
    vertex.pos.y = mesh->mVertices[i].y;
    vertex.pos.z = mesh->mVertices[i].z;

    // Normals
    if (mesh->HasNormals()) {
      vertex.normal.x = mesh->mNormals[i].x;
      vertex.normal.y = mesh->mNormals[i].y;
      vertex.normal.z = mesh->mNormals[i].z;
    } else {
      vertex.normal = glm::vec3(0.0f);
    }

    // Texture Coordinates
    if (mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
      vertex.tex.x = mesh->mTextureCoords[0][i].x;
      vertex.tex.y = mesh->mTextureCoords[0][i].y;
    } else {
      vertex.tex = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }
}
