#include "render_backend.h"

#include "../../logger/logger.h"

#include "../../dorton_util/darray/darray.h"
#include "../../dorton_util/dstring/dstring.h"

#include <GLFW/glfw3.h>

#include <stdlib.h>

const char *validation_layers[] = {"VK_LAYER_KHRONOS_validation"};
const u32 validation_layers_size = 1;

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
  for (int i = 0; i < extension_count; ++i)
  {
    DDEBUG("\t%s", *((const char **)darray_get(&required_extensions, i)))
  }
#endif

  inst_create_info.enabledExtensionCount = extension_count;
  inst_create_info.ppEnabledExtensionNames = darray_data(&required_extensions);

  DArray required_validation_layers;
  u32 required_validation_layers_count = 0;

#if defined(_DEBUG)

  DDEBUG("Required layers:");

  darray_create(&required_validation_layers, const char *);

  for (u32 i = 0; i < validation_layers_size; ++i)
  {
    DDEBUG("\t%s", validation_layers[i]);
    darray_push(&required_validation_layers, validation_layers[i]);
  }

  required_validation_layers_count = required_validation_layers.size;
  u32 available_layers_count;

  DArray available_layers;
  vkEnumerateInstanceLayerProperties(&available_layers_count, NULL);
  darray_reserve(&available_layers, VkLayerProperties, available_layers_count);
  vkEnumerateInstanceLayerProperties(&available_layers_count, (VkLayerProperties *)darray_data(&available_layers));
  available_layers.size = available_layers_count;

  for (u32 i = 0; i < required_validation_layers_count; ++i)
  {
    b8 found = false;
    const char *required_string = *(const char **)darray_get(&required_validation_layers, i);
    for (u32 j = 0; j < available_layers_count; ++j)
    {
      const char *available_string = (*(VkLayerProperties *)darray_get(&available_layers, j)).layerName;
      if (dstring_equal_raw(required_string, available_string))
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      DFATAL("Required validation layer is missing: %s", required_string);
      darray_destroy(&required_validation_layers);
      darray_destroy(&available_layers);
      return D_ERROR;
    }
  }

#endif

  inst_create_info.enabledLayerCount = required_validation_layers_count;
  inst_create_info.ppEnabledLayerNames = darray_data(&required_validation_layers);

  if (vkCreateInstance(&inst_create_info, backend->vulkan_context.allocation_callbacks, &backend->vulkan_context.instance) != VK_SUCCESS)
  {
    darray_destroy(&required_extensions);
    darray_destroy(&required_validation_layers);
    DFATAL("Unable to create render instance.");
    return D_ERROR;
  }

  darray_destroy(&required_extensions);
  darray_destroy(&required_validation_layers);

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