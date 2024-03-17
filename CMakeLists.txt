cmake_minimum_required(VERSION 3.12)
project(Vulkan-Renderer)

# Find the Vulkan package.
find_package(Vulkan REQUIRED)

# Find the GLFW package.
find_package(glfw3 REQUIRED)

# Add the executable that will be built from the source files.
add_executable(Vulkan-Renderer src/main.cpp)

# Include directories for Vulkan-Renderer target
target_include_directories(Vulkan-Renderer PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    # Include GLM library
    ${CMAKE_SOURCE_DIR}/lib/glm
)

# Link the Vulkan and GLFW libraries with the executable.
target_link_libraries(Vulkan-Renderer Vulkan::Vulkan glfw)

message(STATUS "Vulkan include dirs: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "GLFW include dirs: ${GLFW_INCLUDE_DIRS}")
