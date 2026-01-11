#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "drawManager.h"
#include "lightManager.h"
#include "textureManager.h"

class Scene {
public:
  Scene(const std::string &name, TextureManager &textureManager,
        MeshManager &meshManager, LightManager &lightManager);
  virtual ~Scene();

  // Scene lifecycle
  virtual void init() = 0;
  virtual void update(float deltaTime) {}
  virtual void cleanup();

  // Getters
  const std::string &getName() const { return name; }
  DrawManager &getDrawManager() { return drawManager; }
  bool isInitialized() const { return initialized; }

protected:
  std::string name;
  TextureManager &textureManager;
  MeshManager &meshManager;
  LightManager &lightManager;
  DrawManager drawManager;
  bool initialized = false;
};

// Scene manager to handle multiple scenes
class SceneManager {
public:
  SceneManager(TextureManager &textureManager, MeshManager &meshManager,
               LightManager &lightManager);
  ~SceneManager();

  // Register a scene with a name
  void registerScene(const std::string &name, std::unique_ptr<Scene> scene);

  // Load and switch to a scene by name
  bool loadScene(const std::string &name);

  // Get current active scene
  Scene *getActiveScene() const { return activeScene; }

  // Check if a scene exists
  bool hasScene(const std::string &name) const;

  // Get a scene by name (without switching)
  Scene *getScene(const std::string &name) const;

private:
  std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
  Scene *activeScene = nullptr;

  TextureManager &textureManager;
  MeshManager &meshManager;
  LightManager &lightManager;
};