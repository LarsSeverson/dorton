#include "render_backend.h"

#include "logger.h"

#include "darray/darray.h"
#include "dstring/dstring.h"

#include "./utils/render_backend_utils_debug.h"

#include <GLFW/glfw3.h>

DResult renderer_backend_create_instance(RenderBackend *backend)
{
  VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app_info.pApplicationName = backend->app_title;
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Dorton Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo inst_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  inst_create_info.pApplicationInfo = &app_info;

  DArray required_extensions;
  darray_create(&required_extensions, const char *);

  u32 extensions_count = 0;
  const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

  for (int i = 0; i < extensions_count; ++i)
  {
    darray_push(&required_extensions, glfw_extensions[i]);
  }

#if defined(_DEBUG)
  get_debug_extensions(&required_extensions, &extensions_count);
#endif

  inst_create_info.enabledExtensionCount = extensions_count;
  inst_create_info.ppEnabledExtensionNames = darray_data(&required_extensions);

  DArray required_validation_layers;
  u32 required_validation_layers_count = 0;

#if defined(_DEBUG)
  get_debug_layers(&required_validation_layers, &required_validation_layers_count);
#endif

  inst_create_info.enabledLayerCount = required_validation_layers_count;
  inst_create_info.ppEnabledLayerNames = darray_data(&required_validation_layers);

  VkResult result = vkCreateInstance(&inst_create_info, backend->vulkan_context.allocator, &backend->vulkan_context.instance);

  darray_destroy(&required_extensions);
#if defined(_DEBUG)
  darray_destroy(&required_validation_layers);
#endif

  if (result != D_SUCCESS)
  {
    DFATAL("Unable to create render instance.");
    return D_FATAL;
  }

  return D_SUCCESS;
}

DResult render_backend_create_surface(RenderBackend *backend)
{
  if (glfwCreateWindowSurface(backend->vulkan_context.instance, backend->window->window, backend->vulkan_context.allocator, &backend->vulkan_context.surface) != D_SUCCESS)
  {
    DFATAL("Unable to create render surface.");
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult render_backend_create(RenderBackend *backend, RenderBackendCreateInfo *create_info)
{
  backend->app_title = create_info->app_title;
  backend->window = create_info->window;

  DINFO("Creating render backend:");
  if (renderer_backend_create_instance(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend instance created.");

  if (render_backend_create_surface(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend surface created.");

  if (render_backend_create_device(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend device created.");

  if (render_backend_create_swap_chain(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend swap chain created.");

  if (render_backend_create_render_pass(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend render pass created.");

  if (render_backend_create_command_pool(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend command pool created.");

  if (render_backend_create_command_buffers(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend command buffers created.");

  if (render_backend_create_framebuffers(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend framebuffers created.");

  if (render_backend_create_fences(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend fences created.");

  if (render_backend_create_semaphores(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend semaphores created.");

  DINFO("Render backend created.");

  return D_SUCCESS;
}

DResult render_backend_destroy(RenderBackend *backend)
{
  if (backend->vulkan_context.debug_messenger)
  {
    PFN_vkDestroyDebugUtilsMessengerEXT vk_debugger_destroy_func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(backend->vulkan_context.instance, "vkDestroyDebugUtilsMessengerEXT");
    vk_debugger_destroy_func(backend->vulkan_context.instance, backend->vulkan_context.debug_messenger, backend->vulkan_context.allocator);
  }

  // Semaphores
  render_backend_destroy_semaphores(backend);

  // Fences
  render_backend_destroy_fences(backend);

  // Framebuffers
  render_backend_destroy_framebuffers(backend);

  // Command buffers
  render_backend_destroy_command_buffers(backend);

  // Command pool
  render_backend_destroy_command_pool(backend);

  // Render pass
  render_backend_destroy_render_pass(backend);

  // Swap chain
  render_backend_destroy_swap_chain(backend);

  // Device (Physical / Logical)
  render_backend_destroy_device(backend);

  // Surface
  vkDestroySurfaceKHR(backend->vulkan_context.instance, backend->vulkan_context.surface, backend->vulkan_context.allocator);

  // Instance
  vkDestroyInstance(backend->vulkan_context.instance, backend->vulkan_context.allocator);

  return D_SUCCESS;
}

DResult render_backend_begin_frame(RenderBackend *backend, f32 delta_time)
{
  return D_SUCCESS;
}

DResult render_backend_end_frame(RenderBackend *backend, f32 delta_time)
{
  return D_SUCCESS;
}