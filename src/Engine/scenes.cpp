#include "scenes.h"
#include "assetImportManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// ============ Default Scene ============

DefaultScene::DefaultScene(TextureManager &textureManager,
                           MeshManager &meshManager, LightManager &lightManager)
    : Scene("default", textureManager, meshManager, lightManager) {}

void DefaultScene::init() {
  std::cout << "Initializing Default Scene..." << std::endl;

  // Create ground plane with texture
  auto groundMaterial = std::make_shared<Material>(MaterialType::TEXTURED);
  groundMaterial->setColor(
      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // White - no tint on texture

  try {
    textureManager.loadTexture("../src/Assets/Textures/groundTex.jpg");
    groundMaterial->setTexture(textureManager.getTextureImageView(),
                               textureManager.getTextureSampler());
  } catch (const std::exception &e) {
    std::cerr << "Failed to load texture: " << e.what() << std::endl;
  }

  Mesh *plane = drawManager.drawPlane(glm::vec3(0.0f, -1.0f, 0.0f), 10.0f);
  drawManager.setMaterial(plane, groundMaterial);

  // --- Human Figure ---

  // Materials
  auto skinMaterial = std::make_shared<Material>(MaterialType::COLORED);
  skinMaterial->setColor(glm::vec4(1.0f, 0.8f, 0.6f, 1.0f)); // Skin tone

  auto shirtMaterial = std::make_shared<Material>(MaterialType::COLORED);
  shirtMaterial->setColor(glm::vec4(0.2f, 0.2f, 0.8f, 1.0f)); // Blue shirt

  auto pantsMaterial = std::make_shared<Material>(MaterialType::COLORED);
  pantsMaterial->setColor(glm::vec4(0.1f, 0.1f, 0.4f, 1.0f)); // Dark blue pants

  // 1. Torso (Rectangle)
  // Scale of cube defaults to 1.0. We want 0.8 x 1.2 x 0.4
  Mesh *torso = drawManager.drawCube(glm::vec3(0.0f, 1.0f, 0.0f),
                                     glm::vec3(0.8f, 1.2f, 0.4f));
  drawManager.setMaterial(torso, shirtMaterial);

  // 2. Head (Sphere)
  // Positioned above torso (Torso top = 1.6). Head center at 2.1
  Mesh *head = drawManager.drawSphere(glm::vec3(0.0f, 2.1f, 0.0f), 0.4f);
  drawManager.setMaterial(head, skinMaterial);

  // 3. Legs (Capsules)
  // Torso bottom is at 0.4.
  // Legs should extend from 0.4 down to near -1.0.
  // Length approx 1.2. Radius 0.15.
  // Center y = 0.4 - (0.6 + 0.15) ? No, let's explicit place.
  // Ground is -1.0. Hip is 0.4. Length needed: 1.4.
  // Capsule total height = h + 2r. 1.4 = h + 0.3 -> h = 1.1.
  // Center pos = -1.0 + 0.7 = -0.3.
  float legRadius = 0.15f;
  float legCylHeight = 1.1f;
  float legY = -0.3f;

  Mesh *leftLeg = drawManager.drawCapsule(glm::vec3(-0.25f, legY, 0.0f),
                                          legRadius, legCylHeight);
  drawManager.setMaterial(leftLeg, pantsMaterial);

  Mesh *rightLeg = drawManager.drawCapsule(glm::vec3(0.25f, legY, 0.0f),
                                           legRadius, legCylHeight);
  drawManager.setMaterial(rightLeg, pantsMaterial);

  // 4. Arms (Capsules)
  // Shoulder at approx 1.5. Hand down.
  float armRadius = 0.12f;
  float armCylHeight = 0.9f;
  float armY = 1.0f; // Beside torso

  Mesh *leftArm = drawManager.drawCapsule(glm::vec3(-0.55f, armY, 0.0f),
                                          armRadius, armCylHeight);
  drawManager.setMaterial(leftArm, skinMaterial);

  Mesh *rightArm = drawManager.drawCapsule(glm::vec3(0.55f, armY, 0.0f),
                                           armRadius, armCylHeight);
  drawManager.setMaterial(rightArm, skinMaterial);

  // 5. Cape (Plane with Moving Shader)
  auto capeMaterial = std::make_shared<Material>(MaterialType::COLORED_MOVING);
  capeMaterial->setColor(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)); // Red cape

  // Plane size 0.4 -> Width 0.8, Height 0.8 (initially depth).
  // We want it longer? Let's scale it.
  Mesh *cape = drawManager.drawPlane(glm::vec3(0.0f, 0.0f, 0.0f), 0.4f);

  // Transform: Position behind neck/shoulders, Rotate vertical, Scale length?
  glm::mat4 capeModel = glm::mat4(1.0f);
  // Position: centered X, Y roughly shoulders (1.6) down to knees?
  // Only translation is handled here if we use drawPlane(0,0,0).
  // But drawPlane sets position in vertices? No, it takes position arg.
  // Let's create at Origin and move via Model Matrix to handle rotation
  // properly. Position arguments in drawPlane displace vertices. Model Matrix
  // transforms them. It is safer to use Model Matrix for complex transforms
  // (Rotation). Initial Plane is XZ. Normal (0,1,0). Rotate 90 deg X -> XY
  // Plane. Normal (0,0,1). Facing +Z. We want it behind character (at -Z),
  // facing backward? or double sided culling? Culling is back bit. Plane
  // usually has one side. If we rotate +90, Normal points +Z? or -Z? (0,1,0) x
  // RotX(90) -> (0,0,1). Character is at Z=0. Back is -Z. So Normal (0,0,1)
  // points towards character. This is "Back Face" if viewing from behind? If I
  // look from +Z (front), I see front of char. Cape is behind. If I rotate
  // camera to back (-Z), I see cape. So cape normal should point -Z (away from
  // character). Rotate -90 deg X? (0,1,0) -> (0,0,-1).
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //

  capeModel = glm::translate(
      capeModel,
      glm::vec3(0.0f, 0.37f, -0.63f)); // Adjusted for angle and length
  capeModel =
      glm::rotate(capeModel, glm::radians(110.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  capeModel = glm::scale(
      capeModel, glm::vec3(1.0f, 1.0f, 3.0f)); // Make it even longer
                                               // space becomes Height in world)

  cape->setModelMatrix(capeModel);
  drawManager.setMaterial(cape, capeMaterial);

  // Setup lights
  // Wrapped Diffuse (Half-Lambert) will handle visibility in shadows.
  lightManager.setDirectionalLight(glm::vec3(-2.0f, -4.0f, -0.6f),
                                   glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
  lightManager.setAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);

  // --- Sword Loading ---
  // Import Blender model for the sword
  AssetImportManager assetImporter;
  void *loadedAsset = assetImporter.importAsset(
      "../src/Assets/Models/sword/dwarvensword.blend.blend", AssetType::Model);

  if (loadedAsset) {
    std::vector<Vertex> *swordVertices =
        static_cast<std::vector<Vertex> *>(loadedAsset);

    if (!swordVertices->empty()) {
      Mesh *swordMesh = meshManager.createMesh(*swordVertices);

      // Create a textured material
      auto swordMaterial = std::make_shared<Material>(MaterialType::TEXTURED);
      swordMaterial->setColor(glm::vec4(1.0f)); // White tint

      try {
        textureManager.loadTexture(
            "../src/Assets/Models/sword/Dwarvensword.png");
        swordMaterial->setTexture(textureManager.getTextureImageView(),
                                  textureManager.getTextureSampler());
      } catch (const std::exception &e) {
        std::cerr << "Failed to load sword texture: " << e.what() << std::endl;
        // Fallback color if texture fails (though unlikely if file exists)
        swordMaterial = std::make_shared<Material>(MaterialType::COLORED);
        swordMaterial->setColor(glm::vec4(0.8f, 0.8f, 0.9f, 1.0f));
      }

      // Transform sword to hand
      // Hand is at (0.55f, armY - length/2, 0.0f) approx.
      // Right Arm: (0.55f, 1.0f, 0.0f), Length 0.9f (+ caps).
      // Bottom of arm cylinder is at 1.0f - 0.45f = 0.55f.
      // Sword handle should be around there.
      // Sword model might need scaling/rotation.

      glm::mat4 swordModel = glm::mat4(1.0f);
      swordModel = glm::translate(swordModel, glm::vec3(0.55f, 0.45f, 0.0f));

      // Transformations to align with hand
      swordModel = glm::rotate(swordModel, glm::radians(90.0f),
                               glm::vec3(1.0f, 0.0f, 0.0f)); // Point forward

      // User requested rotation to fix grip. Previous Z-rotation caused
      // sideways pointing. Rotating on Y (Local Handle Axis) before X-Tilt will
      // roll the sword.
      swordModel = glm::rotate(swordModel, glm::radians(90.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));

      swordModel = glm::scale(
          swordModel,
          glm::vec3(
              5.0f)); // User requested 10x (assuming 10x scaling or 5.0 size)

      swordMesh->setModelMatrix(swordModel);
      drawManager.setMaterial(swordMesh, swordMaterial);
    }

    // Cleanup imported raw data
    delete swordVertices;
  } else {
    std::cerr << "Failed to load sword model!" << std::endl;
  }

  // --- Tree Loading ---
  void *treeAsset = assetImporter.importAsset(
      "../src/Assets/Models/trees/oaktree.obj", AssetType::Model);
  // Reusing Model type as it uses generic Assimp logic which works for OBJ too.

  if (treeAsset) {
    std::vector<Vertex> *treeVertices =
        static_cast<std::vector<Vertex> *>(treeAsset);

    if (!treeVertices->empty()) {
      // Create texture for tree
      auto treeMaterial = std::make_shared<Material>(MaterialType::TEXTURED);
      treeMaterial->setColor(glm::vec4(1.0f));
      try {
        textureManager.loadTexture(
            "../src/Assets/Models/trees/oaktree1_bark_BaseColor.jpg");
        treeMaterial->setTexture(textureManager.getTextureImageView(),
                                 textureManager.getTextureSampler());
      } catch (const std::exception &e) {
        std::cerr << "Failed to load tree texture: " << e.what() << std::endl;
      }

      // Positions for 4 corners (assuming ground is approx 10x10 area, corners
      // at +/- 4)
      glm::vec3 treePositions[] = {
          glm::vec3(-9.0f, -1.0f, -9.0f), glm::vec3(9.0f, -1.0f, -9.0f),
          glm::vec3(-9.0f, -1.0f, 9.0f), glm::vec3(9.0f, -1.0f, 9.0f)};

      for (const auto &pos : treePositions) {
        Mesh *treeMesh = meshManager.createMesh(*treeVertices);
        glm::mat4 treeModel = glm::mat4(1.0f);
        treeModel = glm::translate(treeModel, pos);
        // Normalized to 1.0. Scale 12.0 -> 12 meters tall (4x previous 3.0).
        treeModel = glm::scale(treeModel, glm::vec3(12.0f));
        treeMesh->setModelMatrix(treeModel);
        drawManager.setMaterial(treeMesh, treeMaterial);
      }
    }
    delete treeVertices;
  } else {
    std::cerr << "Failed to load tree model!" << std::endl;
  }

  // --- Skybox Loading ---
  void *skyboxAsset = assetImporter.importAsset(
      "../src/Assets/Textures/skybox/free-skybox-enchanted-forest/source/"
      "magic_forest_skybox.fbx",
      AssetType::Model);

  if (skyboxAsset) {
    std::vector<Vertex> *skyboxVertices =
        static_cast<std::vector<Vertex> *>(skyboxAsset);

    if (!skyboxVertices->empty()) {
      Mesh *skyboxMesh = meshManager.createMesh(*skyboxVertices);

      // Create a textured material for the skybox
      auto skyboxMaterial = std::make_shared<Material>(MaterialType::SKYBOX);
      skyboxMaterial->setColor(glm::vec4(1.0f));

      try {
        textureManager.loadTexture(
            "../src/Assets/Textures/skybox/free-skybox-enchanted-forest/"
            "textures/skybox_6k.jpg");
        skyboxMaterial->setTexture(textureManager.getTextureImageView(),
                                   textureManager.getTextureSampler());
      } catch (const std::exception &e) {
        std::cerr << "Failed to load skybox texture: " << e.what() << std::endl;
      }

      glm::mat4 skyboxModel = glm::mat4(1.0f);
      // Move skybox down so the center is roughly at (0,0,0).
      // Since AssetImporter normalizes to [0, 1] on Y and sets pivot to bottom,
      // and we scale by 50, the height is 0 to 50. -25 centers it.
      skyboxModel = glm::translate(skyboxModel, glm::vec3(0.0f, -25.0f, 0.0f));
      // Scale large enough to be a skybox
      skyboxModel = glm::scale(skyboxModel, glm::vec3(50.0f));

      skyboxMesh->setModelMatrix(skyboxModel);
      drawManager.setMaterial(skyboxMesh, skyboxMaterial);
    }
    delete skyboxVertices;
  } else {
    std::cerr << "Failed to load skybox model!" << std::endl;
  }

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
  lightManager.setDirectionalLight(glm::vec3(-1.0f, -2.0f, -1.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f), 1.2f);
  lightManager.setAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);

  initialized = true;
  std::cout << "Demo Scene initialized." << std::endl;
}

void DemoScene::update(float deltaTime) {
  // Demo scene update logic (if any)
}
