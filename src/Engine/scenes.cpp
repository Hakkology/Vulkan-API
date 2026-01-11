#include "scenes.h"
#include <iostream>

// ============ Default Scene ============

DefaultScene::DefaultScene(TextureManager &textureManager,
                           MeshManager &meshManager, LightManager &lightManager)
    : Scene("default", textureManager, meshManager, lightManager) {}

void DefaultScene::init() {
  std::cout << "Initializing Default Scene..." << std::endl;

  // Create ground plane with texture
  auto groundMaterial = std::make_shared<Material>(MaterialType::TEXTURED);
  groundMaterial->setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

  try {
    textureManager.loadTexture("../src/Assets/Textures/groundTex.jpg");
    groundMaterial->setTexture(textureManager.getTextureImageView(),
                               textureManager.getTextureSampler());
  } catch (const std::exception &e) {
    std::cerr << "Failed to load texture: " << e.what() << std::endl;
  }

  Mesh *plane = drawManager.drawPlane(glm::vec3(0.0f, -1.0f, 0.0f), 5.0f);
  drawManager.setMaterial(plane, groundMaterial);

  // Create a cube with colored material
  auto cubeMaterial = std::make_shared<Material>(MaterialType::COLORED);
  cubeMaterial->setColor(glm::vec4(1.0f, 0.5f, 0.31f, 1.0f)); // Orange

  Mesh *cube =
      drawManager.drawCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
  drawManager.setMaterial(cube, cubeMaterial);

  // Setup lights
  lightManager.setDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(-2.0f, -4.0f, -1.0f), 1.0f);
  lightManager.setAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.4f);

  initialized = true;
  std::cout << "Default Scene initialized." << std::endl;
}

void DefaultScene::update(float deltaTime) {
  // Default scene update logic (if any)
}

// ============ Demo Scene ============

DemoScene::DemoScene(TextureManager &textureManager, MeshManager &meshManager,
                     LightManager &lightManager)
    : Scene("demo", textureManager, meshManager, lightManager) {}

void DemoScene::init() {
  std::cout << "Initializing Demo Scene..." << std::endl;

  // Create ground plane
  auto groundMaterial = std::make_shared<Material>(MaterialType::COLORED);
  groundMaterial->setColor(glm::vec4(0.2f, 0.6f, 0.2f, 1.0f)); // Green

  Mesh *plane = drawManager.drawPlane(glm::vec3(0.0f, -1.0f, 0.0f), 8.0f);
  drawManager.setMaterial(plane, groundMaterial);

  // Create multiple cubes in a pattern
  auto redMaterial = std::make_shared<Material>(MaterialType::COLORED);
  redMaterial->setColor(glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));

  auto blueMaterial = std::make_shared<Material>(MaterialType::COLORED);
  blueMaterial->setColor(glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));

  auto yellowMaterial = std::make_shared<Material>(MaterialType::COLORED);
  yellowMaterial->setColor(glm::vec4(1.0f, 1.0f, 0.2f, 1.0f));

  // Red cube - left
  Mesh *cube1 =
      drawManager.drawCube(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(1.0f));
  drawManager.setMaterial(cube1, redMaterial);

  // Blue cube - center
  Mesh *cube2 =
      drawManager.drawCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.5f));
  drawManager.setMaterial(cube2, blueMaterial);

  // Yellow cube - right
  Mesh *cube3 =
      drawManager.drawCube(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.8f));
  drawManager.setMaterial(cube3, yellowMaterial);

  // Setup lights - brighter for demo
  lightManager.setDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(-1.0f, -2.0f, -1.0f), 1.2f);
  lightManager.setAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);

  initialized = true;
  std::cout << "Demo Scene initialized." << std::endl;
}

void DemoScene::update(float deltaTime) {
  // Demo scene update logic (if any)
}
