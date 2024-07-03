#include "render_backend_buffer.h"

#include "logger.h"

#include "./utils/render_backend_buffer_utils.h"

#include "render/render_backend/render_backend.h"
#include "render/render_backend/render_backend_command/render_backend_command_buffer/render_backend_command_buffer.h"

DResult render_backend_create_buffer(RenderBackend *backend, RenderBackendBuffer *buffer, BufferInfo *buffer_info)
{
  VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  buffer_create_info.size = buffer_info->size;
  buffer_create_info.usage = buffer_info->usage;
  buffer_create_info.sharingMode = buffer_info->sharing_mode;

  if (vkCreateBuffer(backend->device.logical_device, &buffer_create_info, backend->vulkan_context.allocator, &buffer->buffer_inner) != VK_SUCCESS)
  {
    DERROR("Could not create buffer.");
    return D_ERROR;
  }

  VkMemoryRequirements memory_requirements;
  vkGetBufferMemoryRequirements(backend->device.logical_device, buffer->buffer_inner, &memory_requirements);

  VkMemoryAllocateInfo allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  allocate_info.allocationSize = memory_requirements.size;
  allocate_info.memoryTypeIndex = find_buffer_memory_type(backend, memory_requirements.memoryTypeBits, buffer_info->properties);
  if (allocate_info.memoryTypeIndex == UINT32_MAX)
  {
    DERROR("Could not create buffer memory. memoryTypeIndex was U32MAX");
    return D_ERROR;
  }

  if (vkAllocateMemory(backend->device.logical_device, &allocate_info, backend->vulkan_context.allocator, &buffer->memory) != VK_SUCCESS)
  {
    DERROR("Could not create buffer memory.");
    return D_ERROR;
  }

  if (vkBindBufferMemory(backend->device.logical_device, buffer->buffer_inner, buffer->memory, buffer_info->offset) != VK_SUCCESS)
  {
    DERROR("Could not bind buffer memory.");
    return D_ERROR;
  }

  buffer->size = buffer_info->size;
  buffer->offset = buffer_info->offset;

  return D_SUCCESS;
}

DResult render_backend_destroy_buffer(RenderBackend *backend, RenderBackendBuffer *buffer)
{
  vkDestroyBuffer(backend->device.logical_device, buffer->buffer_inner, backend->vulkan_context.allocator);
  vkFreeMemory(backend->device.logical_device, buffer->memory, backend->vulkan_context.allocator);

  *buffer = (RenderBackendBuffer){0};

  return D_SUCCESS;
}

DResult render_backend_copy_buffer(RenderBackend *backend, RenderBackendBuffer *source, RenderBackendBuffer *destination, u64 size)
{
  RenderBackendCommandBuffer command_buffer;
  if (command_buffer_begin_lazy(backend, &command_buffer) != D_SUCCESS)
  {
    DERROR("Could not copy buffer.");
    return D_ERROR;
  }

  VkBufferCopy copy_region = {0};
  copy_region.size = size;
  vkCmdCopyBuffer(command_buffer.command_buffer_inner, source->buffer_inner, destination->buffer_inner, 1, &copy_region);

  if (command_buffer_end_lazy(backend, &command_buffer, backend->device.graphics_queue) != D_SUCCESS)
  {
    DERROR("Could not copy buffer.");
    return D_ERROR;
  }

  return D_SUCCESS;
}
