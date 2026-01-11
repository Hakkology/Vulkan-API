# 🌌 Koprulu Vulkan Renderer

A high-performance, modular 3D renderer built from scratch using the **Vulkan API**. This engine focuses on clean architecture, real-time lighting, and modern graphics techniques.

![Project Preview](https://via.placeholder.com/800x450.png?text=Vulkan+Renderer+Preview) <!-- Replace with an actual screenshot when available -->

## 🚀 Features

-   **Vulkan 1.3 Core**: Leverages modern Vulkan features for efficient GPU utilization.
-   **Shadow Mapping**: Real-time directional shadows with depth-bias optimization to fix shadow acne.
-   **Advanced Material System**:
    -   **Colored**: Simple vertex-colored materials.
    -   **Textured**: High-quality diffuse textures with lighting support.
    -   **Animated (Cape Shader)**: Dynamic vertex displacement for cloth-like movement.
    -   **Skybox**: Optimized unlit pipeline for high-resolution background environments.
-   **Asset Pipeline**: Full support for importing complex models (FBX, OBJ, BLEND) via **Assimp Integration**.
-   **Modular Architecture**: Clean separation of concerns with dedicated managers for Devices, Swapchains, Textures, Meshes, and Scenes.
-   **Scene Management**: Robust system to register, load, and switch between dynamic 3D scenes.
-   **Interactive Camera**: Orbit-style camera with smooth zoom and rotation.

## 🛠 Tech Stack

-   **API**: Vulkan 1.3
-   **Windowing**: GLFW
-   **Mathematics**: GLM
-   **Asset Loading**: Assimp
-   **Language**: C++20
-   **Build System**: CMake

## 🏗 Architecture

The engine follows a **Manager-based Architectural Pattern**, ensuring that each Vulkan sub-system is isolated and maintainable:

-   **`VulkanRenderer`**: The core orchestrator that handles the main loop and high-level initialization.
-   **`DeviceManager`**: Handles physical and logical device selection.
-   **`SwapChainManager`**: Manages the life-cycle of presentation surfaces and image acquisition.
-   **`MeshDrawer`**: The main rendering unit that binds appropriate pipelines and records command buffers.
-   **`AssetImportManager`**: Generic interface for loading geometry and textures from external files.
-   **`SceneManager`**: Handles scene state, allowing for complex object arrangements like the "Default Scene" (Character with sword and animated cape in an enchanted forest).

## 📥 Installation & Build

### Prerequisites
-   Vulkan SDK (1.3+)
-   CMake (3.10+)
-   GLFW & GLM
-   Assimp

### Building
```bash
# Clone the repository
git clone https://github.com/Hakkology/Vulkan-API.git
cd Vulkan-API

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j$(nproc)

# Run the renderer
./Vulkan-Renderer
```

## 🎮 Controls

-   **Rotate**: Hold `Right Mouse Button` and drag.
-   **Zoom**: Use the `Mouse Scroll Wheel`.
-   **Validation Layers**: Run with `--enable-validation-layers` for debugging.

## 📁 Project Structure

-   `src/Engine/`: Core logic and sub-system managers.
-   `src/Pipeline/`: Vulkan graphics pipeline configurations, render passes, and synchronization logic.
-   `src/Shaders/`: GLSL shader source code (compiled to SPIR-V in the build process).
-   `src/Assets/`: 3D models and textures.

---
*Developed by [Hakan](https://github.com/Hakkology)*
