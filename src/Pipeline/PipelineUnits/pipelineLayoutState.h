#include <glm/glm.hpp>
#include <iostream>
#include <vulkan/vulkan.h>

class PipelineLayout {
public:
  PipelineLayout(VkDevice device);
  ~PipelineLayout();

  const VkPipelineLayoutCreateInfo *getLayoutInfo() const;
  VkPipelineLayout getPipelineLayout() const;
  bool createPipelineLayout();

private:
  VkDevice device;
  VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
  VkPipelineLayoutCreateInfo layoutInfo;
  VkPushConstantRange pushConstantRange;

  void setupLayoutInfo();
};