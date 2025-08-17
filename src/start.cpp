
#include "rend.hpp"

void Rend::start() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}
