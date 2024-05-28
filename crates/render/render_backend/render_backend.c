#include "render_backend.h"

#include "../../logger/logger.h"

#include "../../dorton_util/darray/darray.h"
#include "../../dorton_util/dstring/dstring.h"

#include <GLFW/glfw3.h>

#include <stdlib.h>

DResult render_backend_create(RenderBackend *backend, RenderBackendCreateInfo *create_info)
{
  backend->vulkan_context = create_info->vulkan_context;

  VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app_info.pApplicationName = create_info->app_name;
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Dorton Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo inst_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  inst_create_info.pApplicationInfo = &app_info;

  DArray required_extensions;
  darray_create(&required_extensions, const char *);

  u32 extension_count = 0;
  const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);

  for (int i = 0; i < extension_count; ++i)
  {
    darray_push(&required_extensions, glfw_extensions[i]);
  }

#if defined(_DEBUG)
  darray_push(&required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  extension_count++;

  DDEBUG("Required extensions:");
  for (int i = 0; i < extension_count; ++i) {
    DDEBUG(*((const char**)darray_get(&required_extensions, i)))
  }
#endif

  inst_create_info.enabledExtensionCount = extension_count;
  inst_create_info.ppEnabledExtensionNames = darray_data(&required_extensions);
  inst_create_info.enabledLayerCount = 0;
  inst_create_info.ppEnabledLayerNames = 0;
  
  // if (vkCreateInstance(&inst_create_info, backend->vulkan_context.allocation_callbacks, &backend->vulkan_context.instance) != VK_SUCCESS)
  // {
  //   darray_destroy(&required_extensions);
  //   DFATAL("Unable to create render instance.");
  //   return D_ERROR;
  // }

  darray_destroy(&required_extensions);

  return D_SUCCESS;
}

DResult render_backend_destroy(RenderBackend *backend)
{
  vkDestroyInstance(backend->vulkan_context.instance, backend->vulkan_context.allocation_callbacks);
  
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