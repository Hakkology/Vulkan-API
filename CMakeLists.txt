cmake_minimum_required(VERSION 3.7.2)
project(Vulkan-Renderer)

# Find the Vulkan package.
find_package(Vulkan REQUIRED)

# Find the GLFW package.
find_package(glfw3 REQUIRED)

# Include the GLM package. GLM is header-only, so we don't need to link a library.
find_package(glm REQUIRED)

# Specify the include directory
include_directories(include)

# Add the executable that will be built from the source files.
# Ensure the path to main.cpp reflects its location in the src directory.
add_executable(Vulkan-Renderer src/main.cpp)

# Link the Vulkan and GLFW libraries with the executable.
target_link_libraries(Vulkan-Renderer Vulkan::Vulkan glfw)
