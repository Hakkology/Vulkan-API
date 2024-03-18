    /*
    
        Library check code:

        #define GLFW_INCLUDE_VULKAN
        #include <GLFW/glfw3.h>

        #define GLM_FORCE_RADIANS
        #define GLM_FORCE_DEPTH_ZERO_TO_ONE
        #include <glm/glm.hpp>
        #include <glm/mat4x4.hpp>

        int main(){
            
            glfwInit();

            // Setup details about window
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);

            // Query extension count
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            // Get extension properties
            VkExtensionProperties* extensions = new VkExtensionProperties[extensionCount];
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);

            // Print extension names
            for (uint32_t i = 0; i < extensionCount; ++i) {
                std::cout << "Extension " << i << ": " << extensions[i].extensionName << std::endl;
            }

            delete[] extensions;

            // glm vector and matrix tests.
            glm::mat4 testMatrix(1.0f);
            glm::vec4 testVector(1.0f);

            auto testResult = testMatrix * testVector;

            // 12 extensions.
            printf("Extension count: %i\n", extensionCount);

            // To keep window open
            while(!glfwWindowShouldClose(window)){

                glfwPollEvents();
            }

            // to close and terminate glfw.
            glfwDestroyWindow(window);

            glfwTerminate();

            return 0;

        }


    */