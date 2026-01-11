#include "vulkanRenderer.h"

VulkanRenderer::VulkanRenderer() {
  // Constructor implementation (if needed)
}

void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice,
                  VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer &buffer,
                  VkDeviceMemory &bufferMemory) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to create buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  uint32_t memoryTypeIndex = UINT32_MAX;
  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((memRequirements.memoryTypeBits & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      memoryTypeIndex = i;
      break;
    }
  }

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = memoryTypeIndex;

  if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate buffer memory!");
  }

  vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

VulkanRenderer::~VulkanRenderer() { terminate(); }

int VulkanRenderer::init(GLFWwindow *newWindow) {
  window = newWindow;
  std::cout << "Starting initialization..." << std::endl;

  // Initialize validation layer and debug messenger first if needed
  if (validation.getValidationLayerState()) {
    if (!checkValidationLayerSupport({"VK_LAYER_KHRONOS_validation"})) {
      std::cerr << "ERROR: Validation layers requested, but not available!"
                << std::endl;
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
    if (!deviceManager.pickPhysicalDevice(
            instance, surfaceManager->getSurface(), window)) {
      std::cerr << "ERROR: Failed to pick a suitable physical device!"
                << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Creating logical device..." << std::endl;
    if (!deviceManager.createLogicalDevice(surfaceManager->getSurface())) {
      std::cerr << "ERROR: Failed to create logical device!" << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Initializing queue manager..." << std::endl;
    if (!queueManager.init(deviceManager.getLogicalDevice(),
                           deviceManager.getPhysicalDevice(),
                           surfaceManager->getSurface())) {
      std::cerr << "ERROR: Failed to initialize queue manager!" << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Creating swap chain..." << std::endl;
    swapChainManager = std::make_unique<SwapChainManager>(
        deviceManager.getPhysicalDevice(), surfaceManager->getSurface(),
        window);
    if (!swapChainManager->createSwapChain(
            deviceManager.getPhysicalDevice(), deviceManager.getLogicalDevice(),
            surfaceManager->getSurface(), window,
            swapChainManager->getSwapChainDetails(
                deviceManager.getPhysicalDevice(),
                surfaceManager->getSurface()))) {
      std::cerr << "ERROR: Failed to create swap chain!" << std::endl;
      return EXIT_FAILURE;
    }

    if (!updateSwapChainSettings()) {
      std::cerr << "ERROR: Failed to update swap chain settings!" << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Initializing Depth Stencil Manager..." << std::endl;
    depthStencilManager = std::make_unique<VulkanDepthStencilManager>(
        deviceManager.getPhysicalDevice(), deviceManager.getLogicalDevice());
    depthStencilManager->createDepthResources(
        swapChainManager->getChosenExtent(), swapChainManager.get());

    std::cout << "Initializing Shadow Map Manager..." << std::endl;
    shadowMapManager = std::make_unique<VulkanShadowMapManager>(
        deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice());
    shadowMapManager->createResources(2048, 2048);
    shadowMapManager->createRenderPass();
    shadowMapManager->createFramebuffer();

    std::cout << "Creating render pass..." << std::endl;
    renderPass = std::make_unique<Renderpass>(
        deviceManager.getLogicalDevice(), swapChainImageFormat,
        depthStencilManager->getDepthFormat());
    renderPass->createRenderPass(); // Create the render pass

    std::cout << "Creating global descriptor set layout..." << std::endl;
    // Binding 0: Global UBO
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags =
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    // Binding 1: Shadow Map
    VkDescriptorSetLayoutBinding shadowLayoutBinding{};
    shadowLayoutBinding.binding = 1;
    shadowLayoutBinding.descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    shadowLayoutBinding.descriptorCount = 1;
    shadowLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    shadowLayoutBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 2> globalBindings = {
        uboLayoutBinding, shadowLayoutBinding};
    VkDescriptorSetLayoutCreateInfo globalLayoutInfo{};
    globalLayoutInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    globalLayoutInfo.bindingCount =
        static_cast<uint32_t>(globalBindings.size());
    globalLayoutInfo.pBindings = globalBindings.data();

    if (vkCreateDescriptorSetLayout(deviceManager.getLogicalDevice(),
                                    &globalLayoutInfo, nullptr,
                                    &globalDescriptorSetLayout) != VK_SUCCESS) {
      throw std::runtime_error(
          "Failed to create global descriptor set layout!");
    }

    std::cout << "Creating descriptor set layout for texture..." << std::endl;
    // Binding 0: Combined image sampler for texture (NOW IN SET 1)
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo textureLayoutInfo{};
    textureLayoutInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    textureLayoutInfo.bindingCount = 1;
    textureLayoutInfo.pBindings = &samplerLayoutBinding;

    if (vkCreateDescriptorSetLayout(
            deviceManager.getLogicalDevice(), &textureLayoutInfo, nullptr,
            &textureDescriptorSetLayout) != VK_SUCCESS) {
      throw std::runtime_error(
          "Failed to create texture descriptor set layout!");
    }

    std::cout << "Creating shadow graphics pipeline..." << std::endl;
    // Cull FRONT faces for shadow pass (render back faces) to fix acne
    shadowPipeline = std::make_unique<GraphicsPipeline>(
        deviceManager.getLogicalDevice(), shadowMapManager->getRenderPass(),
        VkExtent2D{shadowMapManager->getShadowWidth(),
                   shadowMapManager->getShadowHeight()},
        "../src/Shaders/spv/shadow.vert.spv",
        "../src/Shaders/spv/shadow.frag.spv", VK_CULL_MODE_BACK_BIT,
        VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_TRUE);
    shadowPipeline->createGraphicsPipeline(
        {}); // Empty layouts for shadow pass (only push constants)

    std::cout << "Creating main graphics pipelines..." << std::endl;
    graphicsPipeline = std::make_unique<GraphicsPipeline>(
        deviceManager.getLogicalDevice(), renderPass->getRenderPass(),
        swapChainManager->getChosenExtent(),
        "../src/Shaders/spv/colored_shader.vert.spv",
        "../src/Shaders/spv/colored_shader.frag.spv", VK_CULL_MODE_BACK_BIT,
        VK_FRONT_FACE_COUNTER_CLOCKWISE);
    graphicsPipeline->createGraphicsPipeline({globalDescriptorSetLayout});

    texturedGraphicsPipeline = std::make_unique<GraphicsPipeline>(
        deviceManager.getLogicalDevice(), renderPass->getRenderPass(),
        swapChainExtent, "../src/Shaders/spv/textured_shader.vert.spv",
        "../src/Shaders/spv/textured_shader.frag.spv", VK_CULL_MODE_NONE,
        VK_FRONT_FACE_CLOCKWISE);
    texturedGraphicsPipeline->createGraphicsPipeline(
        {globalDescriptorSetLayout, textureDescriptorSetLayout});

    // Global UBO Buffer
    createBuffer(deviceManager.getLogicalDevice(),
                 deviceManager.getPhysicalDevice(), sizeof(GlobalUBO),
                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 globalUboBuffer, globalUboMemory);

    // Initialize Command Manager
    std::cout << "Creating command manager..." << std::endl;
    commandBuffer = std::make_unique<CommandManager>(
        deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice(),
        surfaceManager->getSurface(), graphicsPipeline->getGraphicsPipeline());
    commandBuffer->createCommandPool();

    // Initialize Mesh Manager
    std::cout << "Initializing Mesh Manager..." << std::endl;
    meshManager = std::make_unique<MeshManager>(
        deviceManager.getPhysicalDevice(), deviceManager.getLogicalDevice());

    // Initialize Input Manager
    std::cout << "Initializing Input Manager..." << std::endl;
    inputManager = std::make_unique<InputManager>();
    glfwSetWindowUserPointer(window, inputManager.get());
    inputManager->init(window);

    // Initialize Camera Manager
    std::cout << "Initializing Camera Manager..." << std::endl;
    cameraManager = std::make_unique<CameraManager>();
    inputManager->setCameraManager(cameraManager.get());

    // Initialize Light Manager
    std::cout << "Initializing Light Manager..." << std::endl;
    lightManager = std::make_unique<LightManager>();

    // Initialize Texture Manager
    std::cout << "Initializing Texture Manager..." << std::endl;
    textureManager = std::make_unique<TextureManager>(
        deviceManager.getLogicalDevice(), deviceManager.getPhysicalDevice(),
        commandBuffer->getCommandPool(), queueManager.getGraphicsQueue());

    // Initialize Mesh Drawer
    std::cout << "Creating colored moving graphics pipeline..." << std::endl;
    coloredMovingGraphicsPipeline = std::make_unique<GraphicsPipeline>(
        deviceManager.getLogicalDevice(), renderPass->getRenderPass(),
        swapChainExtent, "../src/Shaders/spv/colored_moving_shader.vert.spv",
        "../src/Shaders/spv/colored_moving_shader.frag.spv",
        VK_CULL_MODE_NONE, // Render both sides
        VK_FRONT_FACE_CLOCKWISE);
    coloredMovingGraphicsPipeline->createGraphicsPipeline(
        {globalDescriptorSetLayout});

    std::cout << "Initializing Mesh Drawer..." << std::endl;
    meshDrawer = std::make_unique<MeshDrawer>(
        deviceManager.getLogicalDevice(), renderPass->getRenderPass(),
        graphicsPipeline->getGraphicsPipeline(),
        texturedGraphicsPipeline->getGraphicsPipeline(),
        coloredMovingGraphicsPipeline->getGraphicsPipeline(),
        graphicsPipeline->getPipelineLayout());

    // Initialize SceneManager
    std::cout << "Initializing Scene Manager" << std::endl;
    sceneManager = std::make_unique<SceneManager>(*textureManager, *meshManager,
                                                  *lightManager);

    // Register scenes
    sceneManager->registerScene(
        "default", std::make_unique<DefaultScene>(*textureManager, *meshManager,
                                                  *lightManager));
    sceneManager->registerScene(
        "demo", std::make_unique<DemoScene>(*textureManager, *meshManager,
                                            *lightManager));

    // Load default scene
    sceneManager->loadScene("default");

    // Initializing Descriptor Pool
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 10;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 20;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 20;

    if (vkCreateDescriptorPool(deviceManager.getLogicalDevice(), &poolInfo,
                               nullptr, &textureDescriptorPool) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create descriptor pool!");
    }

    // Allocate Global Descriptor Set
    VkDescriptorSetAllocateInfo globalAllocInfo{};
    globalAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    globalAllocInfo.descriptorPool = textureDescriptorPool;
    globalAllocInfo.descriptorSetCount = 1;
    globalAllocInfo.pSetLayouts = &globalDescriptorSetLayout;

    if (vkAllocateDescriptorSets(deviceManager.getLogicalDevice(),
                                 &globalAllocInfo,
                                 &globalDescriptorSet) != VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate global descriptor set!");
    }

    // Update Global Descriptor Set
    VkDescriptorBufferInfo uboBufferInfo{};
    uboBufferInfo.buffer = globalUboBuffer;
    uboBufferInfo.offset = 0;
    uboBufferInfo.range = sizeof(GlobalUBO);

    VkDescriptorImageInfo shadowImageInfo{};
    shadowImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    shadowImageInfo.imageView = shadowMapManager->getShadowImageView();
    shadowImageInfo.sampler = shadowMapManager->getShadowSampler();

    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = globalDescriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &uboBufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = globalDescriptorSet;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &shadowImageInfo;

    vkUpdateDescriptorSets(deviceManager.getLogicalDevice(),
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(), 0, nullptr);

    // Allocate descriptor sets for textured meshes
    const auto &meshes = meshManager->getAllMeshes();
    for (const auto &[id, mesh] : meshes) {
      if (mesh && mesh->hasMaterial() && mesh->getMaterial()->hasTexture()) {
        auto material = mesh->getMaterial();

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = textureDescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &textureDescriptorSetLayout;

        VkDescriptorSet descriptorSet;
        if (vkAllocateDescriptorSets(deviceManager.getLogicalDevice(),
                                     &allocInfo,
                                     &descriptorSet) != VK_SUCCESS) {
          throw std::runtime_error("Failed to allocate descriptor set!");
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = material->getTextureImageView();
        imageInfo.sampler = material->getTextureSampler();

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = 0; // Binding 0
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(deviceManager.getLogicalDevice(), 1,
                               &descriptorWrite, 0, nullptr);

        material->setDescriptorSet(descriptorSet);
      }
    }

    std::cout << "Creating frame buffers..." << std::endl;
    frameBuffer = std::make_unique<FrameManager>(
        deviceManager.getLogicalDevice(), *swapChainManager,
        renderPass->getRenderPass());
    frameBuffer->createFrameBuffers(depthStencilManager->getDepthImageViews());

    std::cout << "Allocating command buffers..." << std::endl;
    // Pool already created
    commandBuffer->allocateCommandBuffers(
        frameBuffer->getSwapchainFramebuffers());
    // commandBuffer->recordCommands(frameBuffer->getSwapchainFramebuffers(),
    // renderPass->getRenderPass(), swapChainManager->getChosenExtent(),
    // meshDrawer.get(), meshManager.get());

    std::cout << "Initializing synchronization functionality..." << std::endl;
    // Assuming `frameCount` is defined and represents the number of frames you
    // are managing
    syncHandler = std::make_unique<SynchronizationHandler>(
        deviceManager.getLogicalDevice(), 2);
    syncHandler->createSynchronizationObjects();

  } catch (const std::runtime_error &e) {
    std::cerr << "ERROR during initialization: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Initialization successful" << std::endl;
  return 0;
}

void VulkanRenderer::terminate() {

  // wait until no actions being run on device.
  if (deviceManager.getLogicalDevice() != nullptr) {
    vkDeviceWaitIdle(deviceManager.getLogicalDevice());
  }

  if (meshManager) {
    meshManager.reset();
  }

  if (meshDrawer) {
    meshDrawer.reset();
  }

  if (sceneManager) {
    sceneManager.reset();
  }

  if (inputManager) {
    inputManager.reset();
  }

  if (cameraManager) {
    cameraManager.reset();
  }

  if (lightManager) {
    lightManager.reset();
  }

  if (textureManager) {
    textureManager.reset();
  }

  if (syncHandler) {
    syncHandler->cleanup();
    syncHandler.reset();
  }

  if (commandBuffer) {
    commandBuffer->cleanup();
    commandBuffer.reset();
  }

  if (frameBuffer) {
    frameBuffer->cleanup();
    frameBuffer.reset();
  }

  if (shadowPipeline) {
    shadowPipeline->cleanup();
    shadowPipeline.reset();
  }

  if (graphicsPipeline) {
    graphicsPipeline->cleanup();
    graphicsPipeline.reset();
  }

  if (texturedGraphicsPipeline) {
    texturedGraphicsPipeline->cleanup();
    texturedGraphicsPipeline.reset();
  }

  if (globalDescriptorSetLayout != VK_NULL_HANDLE) {
    vkDestroyDescriptorSetLayout(deviceManager.getLogicalDevice(),
                                 globalDescriptorSetLayout, nullptr);
  }

  if (globalUboBuffer != VK_NULL_HANDLE) {
    vkDestroyBuffer(deviceManager.getLogicalDevice(), globalUboBuffer, nullptr);
  }
  if (globalUboMemory != VK_NULL_HANDLE) {
    vkFreeMemory(deviceManager.getLogicalDevice(), globalUboMemory, nullptr);
  }

  if (shadowMapManager) {
    shadowMapManager->cleanup();
    shadowMapManager.reset();
  }

  if (renderPass) {
    renderPass->cleanup();
    renderPass.reset();
  }

  if (depthStencilManager) {
    depthStencilManager->cleanup();
    depthStencilManager.reset();
  }

  if (swapChainManager) {
    swapChainManager->cleanupSwapChain(deviceManager.getLogicalDevice());
    swapChainManager.reset();
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

void VulkanRenderer::draw() {
  inputManager->update(0.016f);

  cameraManager->updateAspectRatio((float)swapChainExtent.width /
                                   (float)swapChainExtent.height);
  glm::mat4 view = cameraManager->getViewMatrix();
  glm::mat4 projection = cameraManager->getProjectionMatrix();
  glm::mat4 model = glm::mat4(1.0f); // Default model matrix
  glm::mat4 mvp = projection * view * model;

  DirectionalLight dirLight = lightManager->getDirectionalLight();
  glm::vec3 lightPos =
      -glm::vec3(dirLight.direction) * 10.0f; // Position light far away
  glm::mat4 lightProjection =
      glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
  glm::mat4 lightView =
      glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 lightSpaceMatrix = lightProjection * lightView;

  // Update GlobalUBO
  GlobalUBO ubo{};
  ubo.lightSpaceMatrix = lightSpaceMatrix;
  ubo.lightDir = dirLight.direction;
  ubo.lightColor = lightManager->getDirectionalLight().color;
  ubo.ambientLight = lightManager->getAmbientLight();

  ubo.time = (float)glfwGetTime();

  void *data;
  vkMapMemory(deviceManager.getLogicalDevice(), globalUboMemory, 0, sizeof(ubo),
              0, &data);
  memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(deviceManager.getLogicalDevice(), globalUboMemory);

  // Prepare PushConstants
  PushConstants pushConstants = {};
  pushConstants.mvp = mvp;
  pushConstants.objectColor = glm::vec4(1.0f);

  // 1. Get next available image
  vkWaitForFences(deviceManager.getLogicalDevice(), 1,
                  &syncHandler->inFlightFences[currentFrame], VK_TRUE,
                  UINT64_MAX);

  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      deviceManager.getLogicalDevice(), swapChainManager->getSwapchain(),
      UINT64_MAX, syncHandler->imageAvailableSemaphores[currentFrame],
      VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Swap chain is not adequate for presentation.");
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to acquire next image from swap chain!");
  }

  // Reset fences only after we know we are submitting
  vkResetFences(deviceManager.getLogicalDevice(), 1,
                &syncHandler->inFlightFences[currentFrame]);

  // 2. Record Command Buffer

  // Use the command buffer associated with the swapchain image index
  // This ensures we are recording to the buffer that will be submitted
  VkCommandBuffer cb = commandBuffer->getCommandBuffer(imageIndex);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(cb, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin command buffer recording!");
  }

  // Shadow Pass
  commandBuffer->recordShadowPass(
      cb, meshManager.get(), shadowPipeline->getGraphicsPipeline(),
      shadowPipeline->getPipelineLayout(), shadowMapManager->getExtent(),
      shadowMapManager->getRenderPass(), shadowMapManager->getFramebuffer(),
      lightSpaceMatrix);

  // Main Pass
  commandBuffer->recordMainPass(
      cb, frameBuffer->getSwapchainFramebuffers()[imageIndex],
      renderPass->getRenderPass(), swapChainManager->getChosenExtent(),
      meshDrawer.get(), meshManager.get(), pushConstants, globalDescriptorSet);

  if (vkEndCommandBuffer(cb) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end command buffer recording!");
  }

  // 3. Submit
  VkCommandBuffer commandBuffers[] = {
      commandBuffer->getCommandBuffer(imageIndex)};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore waitSemaphores[] = {
      syncHandler->imageAvailableSemaphores[currentFrame]};
  VkSemaphore signalSemaphores[] = {
      syncHandler->renderFinishedSemaphores[currentFrame]};
  VkSwapchainKHR swapchainPtr[] = {swapChainManager->getSwapchain()};

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = commandBuffers;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  result = vkQueueSubmit(queueManager.getGraphicsQueue(), 1, &submitInfo,
                         syncHandler->inFlightFences[currentFrame]);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit command buffer to queue.");
  }

  // 4. Present
  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapchainPtr;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  result = vkQueuePresentKHR(queueManager.getPresentationQueue(), &presentInfo);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present image.");
  }

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
  std::vector<const char *> validationLayers;
  if (validation.getValidationLayerState()) {
    validationLayers.push_back("VK_LAYER_KHRONOS_validation");
  }

  // Resize arrays to correct size
  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();

  // Extensions required by GLFW and potentially by the validation layers
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char *> instanceExtensions(
      glfwExtensions, glfwExtensions + glfwExtensionCount);
  if (validation.getValidationLayerState()) {
    instanceExtensions.push_back(
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Add debug utils extension
  }
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(instanceExtensions.size());
  createInfo.ppEnabledExtensionNames = instanceExtensions.data();
  // Check instance extensions support
  if (!checkInstanceExtensionSupport(&instanceExtensions)) {
    std::cerr << "ERROR: Vulkan Instance does not support required extensions!"
              << std::endl;
    return false;
  }

  // Resize arrays to correct size
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(instanceExtensions.size());
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

bool VulkanRenderer::updateSwapChainSettings() {
  if (swapChainManager) {
    this->swapChainImageFormat = swapChainManager->getChosenFormat();
    this->swapChainExtent = swapChainManager->getChosenExtent();

    if (this->swapChainImageFormat == VK_FORMAT_UNDEFINED ||
        this->swapChainExtent.width == 0 || this->swapChainExtent.height == 0) {
      std::cerr << "Invalid swap chain format or extent values." << std::endl;
      return false; // Return false if the format or extent is not correctly
                    // set.
    }

    std::cout << "Updated swap chain settings." << std::endl;
    return true; // Return true if the update is successful.
  } else {
    std::cerr << "SwapChainManager is not initialized." << std::endl;
    return false; // Return false if the SwapChainManager is not initialized.
  }
}

bool VulkanRenderer::checkInstanceExtensionSupport(
    std::vector<const char *> *checkExtensions) {
  // Need to get number of extensions to create array of correct size to hold
  // extensions
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  // Createa a list of VkExtesionProperties using count
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());

  // Check if given extensions are in list of available extensions
  for (const auto &checkExtension : *checkExtensions) {
    bool hasExtension = false;
    for (const auto &extension : extensions) {
      if (strcmp(checkExtension, extension.extensionName) == 0) {
        hasExtension = true;
        break;
      }
    }

    if (!hasExtension) {
      return false;
    }
  }
  return true;
}

bool VulkanRenderer::checkValidationLayerSupport(
    const std::vector<const char *> &validationLayers) {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
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

void VulkanRenderer::setValidationEnabled() {
  validation.setValidationLayerState(true);
}
