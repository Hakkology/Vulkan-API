#define GLFW_INCLUDE_VULKAN

#include <atomic>
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "vulkanRenderer.h"
#include "window.h"

// Global flag to request application stop
volatile std::sig_atomic_t g_stop_requested = 0;

void signalHandler(int signum) {
  // Determine the signal type and set the flag
  if (signum == SIGINT) {
    g_stop_requested = 1;
  }
}

int main(int argc, char **argv) {

  signal(SIGINT, signalHandler);

  // Create window
  Window window("Vulkan Renderer, 800, 600");

  // Local VulkanRenderer instance
  VulkanRenderer vulkanRenderer;

  // Parse command-line arguments
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--enable-validation-layers") {
      vulkanRenderer.setValidationEnabled();
    }
  }

  // Create Vulkan Renderer instance
  if (vulkanRenderer.init(window.glfwWindow) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  // Loop until closed
  while (!window.shouldClose() && g_stop_requested == 0) {
    window.pollEvents();
    vulkanRenderer.draw();
  }

  vulkanRenderer.terminate();
  return 0;
}