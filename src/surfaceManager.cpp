#include "surfaceManager.h"
#include <iostream>

SurfaceManager::SurfaceManager(VkInstance instance, GLFWwindow* window)
: instance(instance), window(window), surface(VK_NULL_HANDLE) {
    initSurface();
}

SurfaceManager::~SurfaceManager() {
    if (instance != VK_NULL_HANDLE && surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        surface = VK_NULL_HANDLE; // Clear the handle after destruction
    }
}

VkSurfaceKHR SurfaceManager::getSurface() const {
    return surface;
}

void SurfaceManager::initSurface() {
    // Create Surface, create info struct, runs the create surface function, returns result
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
    std::cout << "Surface created successfully." << std::endl;
}