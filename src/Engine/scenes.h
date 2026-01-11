#pragma once

#include "scene.h"

// Default scene with ground plane and cube
class DefaultScene : public Scene {
public:
  DefaultScene(TextureManager &textureManager, MeshManager &meshManager,
               LightManager &lightManager);

  void init() override;
  void update(float deltaTime) override;
};

// Demo scene with multiple objects
class DemoScene : public Scene {
public:
  DemoScene(TextureManager &textureManager, MeshManager &meshManager,
            LightManager &lightManager);

  void init() override;
  void update(float deltaTime) override;
};
