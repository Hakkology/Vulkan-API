#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>



int main (){
    glfwInit();

    // Setup details about window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);

    // extension properties, not stored atm.
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    glm::mat4 testMatrix(1.0f);
    glm::vec4 testVector(1.0f);

    auto testResult = testMatrix * testVector;

    // 12 extensions.
    printf("Extension count: %i\n", extensionCount);

    // To keep window open
    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}