cmake_minimum_required(VERSION 3.12)
project(Vulkan-Renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Vulkan and glfw package.
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Add the executable that will be built from the source files.
add_executable(Vulkan-Renderer
    src/main.cpp

    src/Engine/vulkanValidation.cpp
    src/Engine/deviceManager.cpp
    src/Engine/queueManager.cpp
    src/Engine/surfaceManager.cpp
    src/Engine/swapChainManager.cpp
    src/Pipeline/vulkanRenderpass.cpp
    src/Pipeline/vulkanFrameBuffer.cpp
    src/Pipeline/vulkanCommandBuffer.cpp
    src/Pipeline/vulkanGraphicsPipeline.cpp
    src/Pipeline/vulkanSyncHandler.cpp
    src/Engine/window.cpp
    src/Engine/vulkanUtils.cpp

    src/vulkanRenderer.cpp
)

# Specify directories where the compiler can find headers
target_include_directories(Vulkan-Renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Engine
    ${CMAKE_SOURCE_DIR}/src/Pipeline
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}  # GLM include dizini
)

# Link the Vulkan and GLFW libraries with the executable.
target_link_libraries(Vulkan-Renderer PRIVATE Vulkan::Vulkan glfw)

# # Enable AddressSanitizer
# target_compile_options(Vulkan-Renderer PRIVATE -fsanitize=address -g)
# target_link_options(Vulkan-Renderer PRIVATE -fsanitize=address)

# Shader compilation
add_custom_target(Compile_Shaders
    COMMAND ${CMAKE_COMMAND} -E echo "Compiling shaders..."
    COMMAND bash ${CMAKE_SOURCE_DIR}/src/Shaders/compile_shaders.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/Shaders
    COMMENT "Shaders are being compiled"
)

# Ensure the shaders are compiled before the Vulkan-Renderer is built
add_dependencies(Vulkan-Renderer Compile_Shaders)

message(STATUS "Vulkan include dirs: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "GLFW include dirs: ${GLFW_INCLUDE_DIRS}")
