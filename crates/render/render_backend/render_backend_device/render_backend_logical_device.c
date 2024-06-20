#include "render_backend_logical_device.h"
#include "render_backend_physical_device.h"

#include "logger.h"
#include "darray/darray.h"
#include "dset/dset.h"

DResult render_backend_create_logical_device(RenderBackend *backend)
{
  QueueFamilyIndices indices = {0};
  find_queue_families(&indices, backend->device.physical_device, backend->vulkan_context.surface);

  DArray queue_create_infos;
  DSet unique_queue_families;

  darray_create(&queue_create_infos, VkDeviceQueueCreateInfo);
  dset_create(&unique_queue_families, u32, DSET_DEFAULT_INT_COMP_FUNC);

  dset_push(&unique_queue_families, indices.graphics_family);
  dset_push(&unique_queue_families, indices.present_family);

  f32 queue_priority = 1.f;
  for (u32 i = 0; i < dset_size(&unique_queue_families); ++i)
  {
    VkDeviceQueueCreateInfo queue_create_info = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    queue_create_info.queueFamilyIndex = *(u32 *)dset_get(&unique_queue_families, i);
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;

    darray_push(&queue_create_infos, queue_create_info);
  }

  VkPhysicalDeviceFeatures device_features = {};
  device_features.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo logical_device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  logical_device_create_info.queueCreateInfoCount = (u32)darray_size(&queue_create_infos);
  logical_device_create_info.pQueueCreateInfos = (VkDeviceQueueCreateInfo *)darray_data(&queue_create_infos);
  logical_device_create_info.pEnabledFeatures = &device_features;
  logical_device_create_info.enabledExtensionCount = (u32)get_physical_device_extensions_count();
  logical_device_create_info.ppEnabledExtensionNames = get_physical_device_extensions();

  // depreciated
  logical_device_create_info.enabledLayerCount = 0;
  logical_device_create_info.ppEnabledLayerNames = 0;

  if (vkCreateDevice(backend->device.physical_device, &logical_device_create_info, backend->vulkan_context.allocator, &backend->device.logical_device) != D_SUCCESS)
  {
    DFATAL("Could not create a logical device.");

    darray_destroy(&queue_create_infos);
    dset_destroy(&unique_queue_families);

    return D_FATAL;
  }

  darray_destroy(&queue_create_infos);
  dset_destroy(&unique_queue_families);

  vkGetDeviceQueue(backend->device.logical_device, indices.graphics_family, 0, &backend->device.graphics_queue);
  vkGetDeviceQueue(backend->device.logical_device, indices.present_family, 0, &backend->device.present_queue);

  return D_SUCCESS;
}

DResult render_backend_destroy_logical_device(RenderBackend *backend)
{
  vkDestroyDevice(backend->device.logical_device, backend->vulkan_context.allocator);
  return D_SUCCESS;
}