#define USE_VALIDATION_LAYERS 1

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <array>
#include <vector>
#include <optional>
#include <set>

class Rend {
public:
  Rend();
  void start();
  ~Rend();

  std::array<const char *, 1> validation_layers;
  std::vector<const char *> instance_extensions;
  std::array<const char *, 1> device_extensions;

private:
  GLFWwindow *window;
  VkInstance instance;
  VkPhysicalDevice physical_device;
  VkDevice device;
  VkSurfaceKHR surface;

  // Queue stuff
  std::optional<uint32_t> qfi_graphics;
  std::optional<uint32_t> qfi_present;
  VkQueue queue_graphics;
  VkQueue queue_present;
  bool queue_family_indices_filled() {
    return qfi_graphics.has_value() && qfi_present.has_value();
  }

};
