// Initialization for the Rend class

#include "rend.hpp"

#include <stdexcept>
#include <string.h>
#include <vulkan/vulkan_core.h>

Rend::Rend() {
  puts("start of init");
  validation_layers[0] = "VK_LAYER_KHRONOS_validation";

  // Initialize GLFW
  glfwInitHint(GLFW_PLATFORM_WAYLAND, GLFW_TRUE);
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(800, 600, "Chicken", nullptr, nullptr);
  if (!window) {
    puts("window error");
    throw std::runtime_error("Failed to create GLFW window");
  }

  // Get GLFW instance extensions
  uint32_t glfw_extension_count;
  const char **glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  instance_extensions.resize(glfw_extension_count);
  memcpy(instance_extensions.data(), glfw_extensions,
         glfw_extension_count * sizeof(const char *));

  // Configure application info
  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "rend";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  // Create vulkan instance
  VkInstanceCreateInfo instance_ci{};
  instance_ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_ci.pApplicationInfo = &app_info;
  instance_ci.ppEnabledExtensionNames = instance_extensions.data();
  instance_ci.enabledExtensionCount = instance_extensions.size();
  instance_ci.enabledExtensionCount = 0;
#if USE_VALIDATION_LAYERS == 1
  instance_ci.ppEnabledLayerNames = validation_layers.data();
  instance_ci.enabledLayerCount = validation_layers.size();
#else
  instance_ci.enabledExtensionCount = 0;
#endif
  if (vkCreateInstance(&instance_ci, nullptr, &instance) != VK_SUCCESS)
    throw std::runtime_error("Failed to create vulkan instance");

  // TODO: Verify validation layers
  
  // TODO: Create a debug messenger
  
  // Select a physical device
  uint32_t count_physical_device = 0;
  vkEnumeratePhysicalDevices(instance, &count_physical_device, nullptr);
  if (count_physical_device == 0)
    throw std::runtime_error("Found 0 devices with Vulkan support");
  std::vector<VkPhysicalDevice> vec_physical_device;
  vec_physical_device.resize(count_physical_device);
  vkEnumeratePhysicalDevices(instance, &count_physical_device, vec_physical_device.data());

  // Check devices for suitability
  // TODO: Implement actual suitability checking
  physical_device = vec_physical_device[0];

  // Get the queue family indices
  uint32_t count_queue_family_indices = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count_queue_family_indices, nullptr);
  std::vector<VkQueueFamilyProperties> vec_pd_queue_family_properties;
  vec_pd_queue_family_properties.resize(count_queue_family_indices);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count_queue_family_indices, vec_pd_queue_family_properties.data());
  int i = 0;
  for (const auto& queue_family_properties: vec_pd_queue_family_properties) {
    if (queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      qfi_graphics = i;
    if (queue_family_indices_filled())
      break;
    i++;
  }

  // Create logical device
  float queue_priority = 1.0f;
  VkDeviceQueueCreateInfo ci_device_queue{};
  ci_device_queue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  ci_device_queue.queueFamilyIndex = qfi_graphics.value();
  ci_device_queue.queueCount = 1;
  ci_device_queue.pQueuePriorities = &queue_priority;

  VkPhysicalDeviceFeatures device_features{};
  VkDeviceCreateInfo ci_device{};
  ci_device.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  ci_device.pQueueCreateInfos = &ci_device_queue;
  ci_device.queueCreateInfoCount = 1;
  ci_device.pEnabledFeatures = &device_features;
  ci_device.enabledExtensionCount = 0;
#ifdef USE_VALIDATION_LAYERS
  ci_device.enabledLayerCount = validation_layers.size();
  ci_device.ppEnabledLayerNames = validation_layers.data();
#else
  ci_device.enabledLayerCount = 0;
#endif
  if (vkCreateDevice(physical_device, &ci_device, nullptr, &device) != VK_SUCCESS)
    throw std::runtime_error("Failed to create logical Vulkan device");
  
  // Get queue handles
  vkGetDeviceQueue(device, qfi_graphics.value(), 0, &queue_graphics);
  //vkGetDeviceQueue(device, qfi_present.value(), 0, &queue_present);
  
  puts("end of init");
}

