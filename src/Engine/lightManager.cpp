#include "lightManager.h"

LightManager::LightManager() {
  // Default initialization
  directionalLight.direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
  directionalLight.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  ambientLight = glm::vec4(1.0f, 1.0f, 1.0f, 0.2f); // Default ambient
}

LightManager::~LightManager() {}

void LightManager::setDirectionalLight(const glm::vec3 &direction,
                                       const glm::vec3 &color,
                                       float intensity) {
  directionalLight.direction = glm::vec4(glm::normalize(direction), 0.0f);
  directionalLight.color = glm::vec4(color, intensity);
}

DirectionalLight LightManager::getDirectionalLight() const {
  return directionalLight;
}

void LightManager::setAmbientLight(const glm::vec3 &color, float intensity) {
  ambientLight = glm::vec4(color, intensity);
}

glm::vec4 LightManager::getAmbientLight() const { return ambientLight; }
