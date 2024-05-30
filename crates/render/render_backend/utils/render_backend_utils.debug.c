#include "render_backend_utils_debug.h"

#include "asserts.h"

#include "../../../logger/logger.h"
#include "../../../dorton_util/dstring/dstring.h"

const char *validation_layers[] = {"VK_LAYER_KHRONOS_validation"};
const u32 validation_layers_size = 1;

DResult get_debug_extensions(DArray *required_extensions, u32 *extensions_count)
{
  darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  (*extensions_count)++;

  DDEBUG("Required extensions:");
  for (int i = 0; i < *extensions_count; ++i)
  {
    DDEBUG("\t%s", *((const char **)darray_get(required_extensions, i)))
  }

  return D_SUCCESS;
}

DResult get_debug_layers(DArray *required_validation_layers, u32 *required_validation_layers_count)
{
  DDEBUG("Required layers:");

  darray_create(required_validation_layers, const char *);

  for (u32 i = 0; i < validation_layers_size; ++i)
  {
    DDEBUG("\t%s", validation_layers[i]);
    darray_push(required_validation_layers, validation_layers[i]);
  }

  *required_validation_layers_count = required_validation_layers->size;
  u32 available_layers_count;

  DArray available_layers;
  vkEnumerateInstanceLayerProperties(&available_layers_count, NULL);
  darray_reserve(&available_layers, VkLayerProperties, available_layers_count);
  vkEnumerateInstanceLayerProperties(&available_layers_count, (VkLayerProperties *)darray_data(&available_layers));
  available_layers.size = available_layers_count;

  for (u32 i = 0; i < *required_validation_layers_count; ++i)
  {
    b8 found = false;
    const char *required_string = *(const char **)darray_get(required_validation_layers, i);
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
      darray_destroy(required_validation_layers);
      darray_destroy(&available_layers);
      return D_ERROR;
    }
  }

  return D_SUCCESS;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT types, const VkDebugUtilsMessengerCallbackDataEXT *data, void *user_data);

DResult vk_debug_create(VkInstance *instance, VkAllocationCallbacks *allocator, VkDebugUtilsMessengerEXT *debug_messenger)
{
  u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
  debug_create_info.messageSeverity = log_severity;
  debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
  debug_create_info.pfnUserCallback = vk_debug_callback;

  PFN_vkCreateDebugUtilsMessengerEXT vk_debug_create_func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT");
  DASSERT_MSG(vk_debug_create_func, "Failed to create debug messenger.");

  if (vk_debug_create_func(*instance, &debug_create_info, allocator, debug_messenger) != VK_SUCCESS)
  {
    DDEBUG("Vk debugger could not be created.");
    return D_ERROR;
  }

  DDEBUG("Vk debugger created.");
  return D_SUCCESS;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT types, const VkDebugUtilsMessengerCallbackDataEXT *data, void *user_data)
{
  DDEBUG("Vk debugger:");
  switch (severity)
  {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    DERROR("\t%s", data->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    DWARN("\t%s", data->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    DINFO("\t%s", data->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    DTRACE("\t%s", data->pMessage);
    break;
  default:
    DDEBUG("\tUnknown severity.");
    break;
  }

  return VK_FALSE;
}