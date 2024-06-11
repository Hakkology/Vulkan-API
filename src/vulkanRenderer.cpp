#include "vulkanRenderer.h"  


VulkanRenderer::VulkanRenderer() {
    // Constructor implementation (if needed)
}

VulkanRenderer::~VulkanRenderer() {
    terminate();
}

int VulkanRenderer::init(GLFWwindow* newWindow) {
    window = newWindow;
    std::cout << "Starting initialization..." << std::endl;

    // Initialize validation layer and debug messenger first if needed
    if (validation.getValidationLayerState()) {
        if (!checkValidationLayerSupport({"VK_LAYER_KHRONOS_validation"})) {
            std::cerr << "ERROR: Validation layers requested, but not available!" << std::endl;
            return EXIT_FAILURE;
        }
    }

    try {
        std::cout << "Creating Vulkan instance..." << std::endl;
        if (!createInstance()) {
            std::cerr << "ERROR: Failed to create Vulkan instance!" << std::endl;
            return EXIT_FAILURE;
        }
        
        // Set up debug messenger if validation layers are enabled
        std::cout << "Initializing validation module..." << std::endl;
        if (validation.getValidationLayerState()) {
            if (!validation.setupDebugMessenger(instance)) {
                std::cerr << "ERROR: Failed to set up debug messenger!" << std::endl;
                return EXIT_FAILURE;
            }
        }

        std::cout << "Creating surface..." << std::endl;
        surfaceManager = std::make_unique<SurfaceManager>(instance, window);
        if (!surfaceManager || !surfaceManager->getSurface()) {
            std::cerr << "ERROR: Failed to create surface!" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Picking physical device..." << std::endl;
        if (!deviceManager.pickPhysicalDevice(instance, surfaceManager->getSurface(), window)) {
            std::cerr << "ERROR: Failed to pick a suitable physical device!" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Creating logical device..." << std::endl;
        if (!deviceManager.createLogicalDevice(surfaceManager->getSurface())) {
            std::cerr << "ERROR: Failed to create logical device!" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Initializing queue manager..." << std::endl;
        if (!queueManager.init(deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice(), surfaceManager->getSurface())) {
            std::cerr << "ERROR: Failed to initialize queue manager!" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Creating swap chain..." << std::endl;
        swapChainManager = std::make_unique<SwapChainManager>(deviceManager.getPhysicalDevice(), surfaceManager->getSurface(), window);
        if (!swapChainManager->createSwapChain(deviceManager.getPhysicalDevice(), deviceManager.getLogicalDevice(), surfaceManager->getSurface(), window, 
        swapChainManager->getSwapChainDetails(deviceManager.getPhysicalDevice(), surfaceManager->getSurface()))) {
            std::cerr << "ERROR: Failed to create swap chain!" << std::endl;
            return EXIT_FAILURE;
        }

        if (!updateSwapChainSettings()) {
            std::cerr << "ERROR: Failed to update swap chain settings!" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Creating render pass..." << std::endl;
        renderPass = std::make_unique<Renderpass>(deviceManager.getLogicalDevice(), swapChainImageFormat);
        renderPass->createRenderPass();  // Create the render pass
        
        std::cout << "Creating graphics pipeline..." << std::endl;
        graphicsPipeline =std::make_unique<GraphicsPipeline>(deviceManager.getLogicalDevice(), renderPass->getRenderPass(), swapChainManager->getChosenExtent());
        graphicsPipeline->createGraphicsPipeline();

        std::cout << "Initializing Mesh Manager..." << std::endl;
        auto meshManager = std::make_unique<MeshManager>(deviceManager.getPhysicalDevice(), deviceManager.getLogicalDevice());

        // MeshDrawer initialization
        std::cout << "Initializing Mesh Drawer..." << std::endl;
        auto meshDrawer = std::make_unique<MeshDrawer>(deviceManager.getLogicalDevice(), renderPass->getRenderPass(), graphicsPipeline->getGraphicsPipeline());

        std::cout << "Initializing graphics" << std::endl;
        graphics = std::make_unique<GraphicsInitializer>(instance, deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice());
        graphics->addInitialMeshes(*meshManager);

        std::cout << "Creating frame buffers..." << std::endl;
        frameBuffer = std::make_unique<FrameManager>(deviceManager.getLogicalDevice(), *swapChainManager, renderPass->getRenderPass());
        frameBuffer->createFrameBuffers();

        std::cout << "Creating command buffers..." << std::endl;
        commandBuffer = std::make_unique<CommandManager>(deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice(), surfaceManager->getSurface(), graphicsPipeline->getGraphicsPipeline());
        commandBuffer->createCommandPool();
        commandBuffer->allocateCommandBuffers(frameBuffer->getSwapchainFramebuffers());
        commandBuffer->recordCommands(frameBuffer->getSwapchainFramebuffers(), renderPass->getRenderPass(), swapChainManager->getChosenExtent(), meshDrawer.get(), meshManager.get());
        
        std::cout << "Initializing synchronization functionality..." << std::endl;
        // Assuming `frameCount` is defined and represents the number of frames you are managing
        syncHandler = std::make_unique<SynchronizationHandler>(deviceManager.getLogicalDevice(), 2);
        syncHandler->createSynchronizationObjects();
        
    } catch (const std::runtime_error &e) {
        std::cerr << "ERROR during initialization: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    std::cout << "Initialization successful" << std::endl;
    return 0;
}




void VulkanRenderer::terminate(){

    // wait until no actions being run on device.
    if (deviceManager.getLogicalDevice() != nullptr)
    {
        vkDeviceWaitIdle(deviceManager.getLogicalDevice());
    }
    
    if (meshDrawer)
    {
        meshDrawer.reset();
    }
    
    if (meshManager)
    {
        meshManager.reset();
    }

    if (graphics)
    {
        graphics.reset();
    }
    

    if (syncHandler)
    {
        syncHandler->cleanup();
        syncHandler.reset();
    }
    
    if (commandBuffer)
    {
        commandBuffer->cleanup();
        commandBuffer.reset();
    }
    
    if (frameBuffer)
    {
        frameBuffer->cleanup();
        frameBuffer.reset();
    }
    
    if (graphicsPipeline) {
        graphicsPipeline->cleanup();
        graphicsPipeline.reset();  
    }

    if (renderPass)
    {
        renderPass->cleanup();
        renderPass.reset();
    }
    
    if (swapChainManager) {
        swapChainManager->cleanupSwapChain(deviceManager.getLogicalDevice());  
        swapChainManager.reset();  // Reset the unique_ptr, effectively destroying the SwapChainManager
    }

    if (deviceManager.getLogicalDevice() != VK_NULL_HANDLE) {
        vkDestroyDevice(deviceManager.getLogicalDevice(), nullptr);
        deviceManager.clearLogicalDevice(); // Ensure the handle is cleared
    }

    if (surfaceManager) {
        surfaceManager.reset();
    }

    if (validation.getValidationLayerState()) {
        validation.cleanup(instance);
    }

    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE; // Ensure the handle is cleared
    }
}


void VulkanRenderer::draw()
{
    // 1. Get next available image to draw to and set something to signal when we are finished with the image.
    vkWaitForFences(deviceManager.getLogicalDevice(), 1, &syncHandler->inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(deviceManager.getLogicalDevice(), 1, &syncHandler->inFlightFences[currentFrame]);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(deviceManager.getLogicalDevice(), swapChainManager->getSwapchain(), UINT64_MAX, syncHandler->imageAvailableSemaphores[currentFrame], syncHandler->inFlightFences[currentFrame], &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        // Handle swap chain recreation or handle as needed
        throw std::runtime_error("Swap chain is not adequate for presentation.");
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to acquire next image from swap chain!");
    }

    // 2. Submit command buffer to queue for execution, making sure it waits for the image to be signalled as available before drawing and signal is received.

    // Wait on the current fence before using the image
    // Repeated just before command buffer submission to confirm that all previous operations associated with this frame (like image acquisition) have completed 
    // and the resources are truly ready for reuse.
    vkWaitForFences(deviceManager.getLogicalDevice(), 1, &syncHandler->inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(deviceManager.getLogicalDevice(), 1, &syncHandler->inFlightFences[currentFrame]);

    VkCommandBuffer commandBuffers[] = { commandBuffer->getCommandBuffer(imageIndex) };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore waitSemaphores[] = { syncHandler->imageAvailableSemaphores[currentFrame] };
    VkSemaphore signalSemaphores[] = { syncHandler->renderFinishedSemaphores[currentFrame] };
    VkSwapchainKHR swapchainPtr[] = { swapChainManager->getSwapchain() };

    // Queue submission information

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;                          // number of semaphores to wait on
    submitInfo.pWaitSemaphores = waitSemaphores;                // list of semaphores to wait on.
    submitInfo.pWaitDstStageMask = waitStages;                  // stages to check semaphores at.
    submitInfo.commandBufferCount = 1;                          // number of command buffers to submit.
    submitInfo.pCommandBuffers = commandBuffers;                // command buffer to submit.
    submitInfo.signalSemaphoreCount = 1;                        // number of semaphores to signal.
    submitInfo.pSignalSemaphores = signalSemaphores;            //semaphores to signal when command buffer finishes.

    result = vkQueueSubmit(queueManager.getGraphicsQueue(), 1, &submitInfo, syncHandler->inFlightFences[currentFrame]);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit command buffer to queue.");
    }

    // 3. Present image to screen when it has signalled finished rendering.
    
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;                             // number of semaphores to wait on.
    presentInfo.pWaitSemaphores = signalSemaphores;                 // list of semaphores to wait on.
    presentInfo.swapchainCount = 1;                                 // amount of swapchains.
    presentInfo.pSwapchains = swapchainPtr;                         // swapchain itself.
    presentInfo.pImageIndices = &imageIndex;                        // image index in swapchain to be presented.
    presentInfo.pResults = nullptr; 

    // Present Image
    result = vkQueuePresentKHR(queueManager.getPresentationQueue(), &presentInfo);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present image.");
    }

    // Move to the next frame
    currentFrame = (currentFrame + 1) % syncHandler->frameCount;
}

bool VulkanRenderer::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Koprulu";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Validation layers
    std::vector<const char*> validationLayers;
    if (validation.getValidationLayerState()) {
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
    }
    
    // Resize arrays to correct size
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    // Extensions required by GLFW and potentially by the validation layers
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> instanceExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (validation.getValidationLayerState()) {
        instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Add debug utils extension
    }
    createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();
    // Check instance extensions support
    if (!checkInstanceExtensionSupport(&instanceExtensions)) {
        std::cerr << "ERROR: Vulkan Instance does not support required extensions!" << std::endl;
        return false;
    }

    // Resize arrays to correct size
    createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();

    // Create the Vulkan instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to create Vulkan instance!" << std::endl;
        return false;
    }

    std::cout << "Vulkan Instance created successfully." << std::endl;
    return true;
}

bool VulkanRenderer::updateSwapChainSettings()
{
    if (swapChainManager) {
        this->swapChainImageFormat = swapChainManager->getChosenFormat();
        this->swapChainExtent = swapChainManager->getChosenExtent();

        if (this->swapChainImageFormat == VK_FORMAT_UNDEFINED || this->swapChainExtent.width == 0 || this->swapChainExtent.height == 0) {
            std::cerr << "Invalid swap chain format or extent values." << std::endl;
            return false; // Return false if the format or extent is not correctly set.
        }

        std::cout << "Updated swap chain settings." << std::endl;
        return true; // Return true if the update is successful.
    } else {
        std::cerr << "SwapChainManager is not initialized." << std::endl;
        return false; // Return false if the SwapChainManager is not initialized.
    }
}



bool VulkanRenderer::checkInstanceExtensionSupport(std::vector<const char *> *checkExtensions)
{
    // Need to get number of extensions to create array of correct size to hold extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    // Createa a list of VkExtesionProperties using count
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Supported Extensions (" << extensionCount << "):" << std::endl;
    for (const auto& extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

    // Check if given extensions are in list of available extensions
    for (const auto &checkExtension: *checkExtensions)
    {
        bool hasExtension = false;
        for (const auto &extension: extensions)
        {
            if (strcmp(checkExtension, extension.extensionName) == 0)
            {
                hasExtension = true;
                break;
            }
            
        }

        if (!hasExtension)
        {
            return false;
        } 
    }
    return true;
    
}

bool VulkanRenderer::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void VulkanRenderer::setValidationEnabled(){
    validation.setValidationLayerState(true);
}
