
#include "rend.hpp"

Rend::~Rend() {

  vkDestroyDevice(device, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}
