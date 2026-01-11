#include "scene.h"

Scene::Scene(VkInstance instance, VkDevice device,
             VkPhysicalDevice physicalDevice, TextureManager &textureManager)
    : device(device), instance(instance), physicalDevice(physicalDevice),
      textureManager(textureManager) {
  // meshDrawer = new MeshDrawer(device, physicalDevice);
}

Scene::~Scene() {
  // delete meshDrawer;
}

void Scene::addInitialMeshes(MeshManager &meshManager, ShapeType shapeType) {
  std::vector<Vertex> vertices;

  if (shapeType == ShapeType::TRIANGLE) {
    vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // Vertex 1
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 2
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}  // Vertex 3
    };
  } else if (shapeType == ShapeType::CUBE) {
    // Cube vertices (36 vertices for 12 triangles) with Normals and dummy UVs
    vertices = {// Front face
                {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
                {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
                {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

                // Back face
                {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
                {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
                {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
                {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
                {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
                {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

                // Left face
                {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
                {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
                {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

                // Right face
                {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
                {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
                {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

                // Top face
                {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

                // Bottom face
                {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
                {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
                {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
                {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
                {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}};
  } else if (shapeType == ShapeType::PLANE) {
    // Plane vertices (6 vertices for 2 triangles)
    // Sits on XZ plane, y = -1.0 (below cube with gap)
    float y = -1.0f;
    float s = 5.0f; // Size
    vertices = {{{-s, y, s}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                {{s, y, s}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                {{s, y, -s}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                {{-s, y, s}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                {{s, y, -s}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                {{-s, y, -s}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};
  }

  if (!vertices.empty()) {
    if (shapeType == ShapeType::PLANE) {
      Mesh *planeMesh = meshManager.createMesh(vertices);
      if (planeMesh) {
        planeMesh->setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

        // Load texture
        try {
          if (!vertices.empty()) { // Simple check, texture load is independent
                                   // but good to safeguard
            textureManager.loadTexture("../src/Assets/Textures/groundTex.jpg");
            planeMesh->setTexture(textureManager.getTextureImageView(),
                                  textureManager.getTextureSampler());
          }
        } catch (const std::exception &e) {
          std::cerr << "Failed to load texture: " << e.what() << std::endl;
        }
      }
    } else {
      Mesh *newMesh = meshManager.createMesh(vertices);
      if (newMesh) {
        if (shapeType == ShapeType::CUBE) {
          newMesh->setColor(glm::vec4(1.0f, 0.5f, 0.31f, 1.0f)); // Orange
        }
      }
    }
  }
}

void Scene::initLights(LightManager &lightManager) {
  // Directional Light
  // Direction is vec3(-2.0f, -4.0f, -1.0f) - from top left
  // lightManager.setDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f),
  // glm::vec3(-2.0f, -4.0f, -1.0f), 1.0f);
  lightManager.setDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(-2.0f, -4.0f, -1.0f), 1.0f);

  // Ambient Light
  // Color: White (1.0f, 1.0f, 1.0f), Intensity: 0.4f
  lightManager.setAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.4f);
}
