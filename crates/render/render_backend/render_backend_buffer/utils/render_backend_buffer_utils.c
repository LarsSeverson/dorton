#include "render_backend_buffer_utils.h"

u32 find_buffer_memory_type(RenderBackend *backend, u32 type_filter, VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties memory_properties;
  vkGetPhysicalDeviceMemoryProperties(backend->device.physical_device, &memory_properties);

  for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i)
  {
    if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties)
    {
      return i;
    }
  }
  
  return UINT32_MAX;
}