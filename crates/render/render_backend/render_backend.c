#include "render_backend.h"

#include "logger.h"
#include "darray/darray.h"
#include "dstring/dstring.h"

#include "./utils/render_backend_utils_debug.h"

DResult render_backend_create_instance(RenderBackend *backend)
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
  if (glfwCreateWindowSurface(backend->vulkan_context.instance, backend->window->window_inner, backend->vulkan_context.allocator, &backend->vulkan_context.surface) != D_SUCCESS)
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
  if (render_backend_create_instance(backend) != D_SUCCESS)
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

  if (render_backend_create_fences(backend, &backend->in_flight_fences, backend->swap_chain.max_frames_in_flight) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend fences created.");

  if (render_backend_create_semaphores(backend, &backend->image_available_semaphores, backend->swap_chain.max_frames_in_flight) != D_SUCCESS)
  {
    return D_FATAL;
  }

  if (render_backend_create_semaphores(backend, &backend->render_finished_semaphores, backend->swap_chain.max_frames_in_flight) != D_SUCCESS)
  {
    return D_FATAL;
  }

  DINFO("  Backend semaphores created.");

  backend->current_frame = 0;

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
  render_backend_destroy_semaphores(backend, &backend->image_available_semaphores);
  render_backend_destroy_semaphores(backend, &backend->render_finished_semaphores);

  // Fences
  render_backend_destroy_fences(backend, &backend->in_flight_fences);

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
  if (render_backend_wait_for_fences(backend, &backend->in_flight_fences, backend->current_frame, 1, UINT64_MAX) != D_SUCCESS)
  {
    DERROR("Could not begin frame.");
    return D_ERROR;
  }

  u32 image_index = 0;
  RenderBackendSemaphore *image_available_semaphore = semaphores_get(&backend->image_available_semaphores, backend->current_frame);
  VkResult image_aquire_result = vkAcquireNextImageKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, UINT64_MAX, image_available_semaphore->semaphore_inner, NULL, &image_index);

  if (image_aquire_result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    return render_backend_recreate_swap_chain(backend);
  }
  else if (image_aquire_result != VK_SUCCESS && image_aquire_result != VK_SUBOPTIMAL_KHR)
  {
    DERROR("Failed to get swap chain image.");
    return D_ERROR;
  }

  // if (render_backend_reset_fences(backend, &backend->in_flight_fences, backend->current_frame, 1) != D_SUCCESS)
  // {
  //   return D_ERROR;
  // }

  return D_SUCCESS;
}

DResult render_backend_end_frame(RenderBackend *backend, f32 delta_time)
{
  return D_SUCCESS;
}

DResult render_backend_resize(RenderBackend *backend, i32 width, i32 height)
{
  if (render_backend_recreate_swap_chain(backend) != D_SUCCESS)
  {
    return D_FATAL;
  }
  return D_SUCCESS;
}
