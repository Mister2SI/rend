
#include "rend.hpp"
#include <vulkan/vulkan_core.h>

Rend::~Rend() {

  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyDevice(device, nullptr);
  vkDestroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}
