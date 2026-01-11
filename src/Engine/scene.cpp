#include "scene.h"
#include <iostream>

// ============ Scene Base Class ============

Scene::Scene(const std::string &name, TextureManager &textureManager,
             MeshManager &meshManager, LightManager &lightManager)
    : name(name), textureManager(textureManager), meshManager(meshManager),
      lightManager(lightManager), drawManager(meshManager) {}

Scene::~Scene() { cleanup(); }

void Scene::cleanup() {
  // Base cleanup - derived classes can override
  initialized = false;
}

// ============ Scene Manager ============

SceneManager::SceneManager(TextureManager &textureManager,
                           MeshManager &meshManager, LightManager &lightManager)
    : textureManager(textureManager), meshManager(meshManager),
      lightManager(lightManager) {}

SceneManager::~SceneManager() { scenes.clear(); }

void SceneManager::registerScene(const std::string &name,
                                 std::unique_ptr<Scene> scene) {
  scenes[name] = std::move(scene);
  std::cout << "Scene registered: " << name << std::endl;
}

bool SceneManager::loadScene(const std::string &name) {
  auto it = scenes.find(name);
  if (it == scenes.end()) {
    std::cerr << "Scene not found: " << name << std::endl;
    return false;
  }

  // Cleanup current scene if exists
  if (activeScene) {
    activeScene->cleanup();
  }

  // Switch to new scene
  activeScene = it->second.get();

  // Initialize if not already
  if (!activeScene->isInitialized()) {
    std::cout << "Loading scene: " << name << std::endl;
    activeScene->init();
  }

  return true;
}

bool SceneManager::hasScene(const std::string &name) const {
  return scenes.find(name) != scenes.end();
}

Scene *SceneManager::getScene(const std::string &name) const {
  auto it = scenes.find(name);
  if (it != scenes.end()) {
    return it->second.get();
  }
  return nullptr;
}
