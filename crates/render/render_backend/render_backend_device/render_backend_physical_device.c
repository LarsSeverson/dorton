#include "render_backend_physical_device.h"

#include "logger.h"

#include "darray/darray.h"
#include "dset/dset.h"

#include "render/render_backend/render_backend.h"
#include "render/render_backend/render_backend_swap_chain/utils/render_backend_swap_chain_utils.h"

const char *physical_device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
const u32 physical_device_extensions_count = 1;

const char **get_physical_device_extensions()
{
  return physical_device_extensions;
}

const u32 get_physical_device_extensions_count()
{
  return physical_device_extensions_count;
}

u8 check_physical_device_extension_support(const VkPhysicalDevice physical_device)
{
  u32 extension_count = 0;
  vkEnumerateDeviceExtensionProperties(physical_device, NULL, &extension_count, NULL);

  DArray available_extensions;
  darray_reserve(&available_extensions, VkExtensionProperties, extension_count);
  vkEnumerateDeviceExtensionProperties(physical_device, NULL, &extension_count, (VkExtensionProperties *)darray_data(&available_extensions));

  DSet required_extensions;
  dset_create(&required_extensions, const char *, DSET_DEFAULT_STR_COMP_FUNC);

  for (u32 i = 0; i < physical_device_extensions_count; ++i)
  {
    dset_push(&required_extensions, physical_device_extensions[i]);
  }

  for (u32 i = 0; i < available_extensions.size; ++i)
  {
    dset_erase_val(&required_extensions, darray_get(&available_extensions, i));
  }

  u8 required_extensions_empty = dset_empty(&required_extensions);

  darray_destroy(&available_extensions);
  dset_destroy(&required_extensions);

  return required_extensions_empty == 0;
}

DResult find_queue_families(QueueFamilyIndices *indices, const VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
  u32 queue_family_count;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);

  DArray queue_families;
  darray_reserve(&queue_families, VkQueueFamilyProperties, queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, (VkQueueFamilyProperties *)darray_data(&queue_families));

  u32 index = 0;
  for (u32 i = 0; i < queue_family_count; ++i)
  {
    VkQueueFamilyProperties *queue_family = (VkQueueFamilyProperties *)darray_get(&queue_families, i);

    if (queue_family->queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices->graphics_family = index;
      indices->graphics_has_value = true;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, surface, &present_support);

    if (present_support)
    {
      indices->present_family = index;
      indices->present_has_value = true;
    }

    if (queue_family->queueFlags & VK_QUEUE_TRANSFER_BIT)
    {
      indices->transfer_family = index;
      indices->transfer_has_value = true;
    }

    if (queue_family_indices_is_complete(indices))
    {
      break;
    }

    ++index;
  }

  darray_destroy(&queue_families);

  return D_SUCCESS;
}

u8 is_physical_device_suitable(const VkPhysicalDevice physical_device, RenderBackend *backend)
{
  VkPhysicalDeviceProperties physical_device_properties;
  vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);

  VkPhysicalDeviceFeatures physical_device_features;
  vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

  QueueFamilyIndices indices = {0};
  find_queue_families(&indices, physical_device, backend->vulkan_context.surface);

  u8 extensions_supported = check_physical_device_extension_support(physical_device);
  u8 swap_chain_adequate = false;

  if (extensions_supported)
  {
    SwapChainSupportDetails swap_chain_support;
    swap_chain_support_details_create(&swap_chain_support);

    query_swap_chain_support(&swap_chain_support, physical_device, backend->vulkan_context.surface);
    swap_chain_adequate = !swap_chain_support_details_empty(&swap_chain_support);

    swap_chain_support_details_destroy(&swap_chain_support);
  }

  return queue_family_indices_is_complete(&indices) && extensions_supported && swap_chain_adequate;
}

DResult render_backend_pick_physical_device(RenderBackend *backend)
{
  u32 physical_device_count = 0;
  vkEnumeratePhysicalDevices(backend->vulkan_context.instance, &physical_device_count, NULL);

  if (physical_device_count == 0)
  {
    DFATAL("Failed to find GPUs with Vulkan support.");
    return D_FATAL;
  }

  DArray physical_devices;
  darray_reserve(&physical_devices, VkPhysicalDevice, physical_device_count);
  vkEnumeratePhysicalDevices(backend->vulkan_context.instance, &physical_device_count, (VkPhysicalDevice *)darray_data(&physical_devices));

  for (u32 i = 0; i < physical_device_count; ++i)
  {
    VkPhysicalDevice physical_device = *(VkPhysicalDevice *)darray_get(&physical_devices, i);
    if (is_physical_device_suitable(physical_device, backend))
    {
      backend->device.physical_device = physical_device;
      break;
    }
  }

  darray_destroy(&physical_devices);

  if (backend->device.physical_device == NULL)
  {
    DFATAL("Could not find a suitable physical device.");
    return D_FATAL;
  }

  return D_SUCCESS;
}

u8 queue_family_indices_is_complete(QueueFamilyIndices *indices)
{
  return indices->graphics_has_value && indices->present_has_value && indices->transfer_has_value;
}