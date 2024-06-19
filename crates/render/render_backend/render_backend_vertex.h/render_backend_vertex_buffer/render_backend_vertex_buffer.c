#include "render_backend_vertex_buffer.h"
#include "logger.h"

#include "../render_backend_vertex.h"
#include "../render_backend_vertex_2D/render_backend_vertex_2D.h"
#include "../render_backend_vertex_3D/render_backend_vertex_3D.h"

#include "render/render_backend/render_backend.h"

#include <string.h>

DResult render_backend_create_vertex_buffer(RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer, VertexBufferInfo *vertex_buffer_info)
{
  if (darray_size(&vertex_buffer_info->vertices) <= 0)
  {
    DERROR("Cannot create vertex buffer with no vertices.");
    return D_ERROR;
  }


  Vertex *vertex = (Vertex *)darray_get(&vertex_buffer_info->vertices, 0);
  VertexType type = vertex->type;
  u64 size = 0;

  for (u32 i = 0; i < darray_size(&vertex_buffer_info->vertices); ++i)
  {
    Vertex *vertex = (Vertex *)darray_get(&vertex_buffer_info->vertices, i);
    if (vertex->type != type)
    {
      DERROR("Vertex %p has conflicting type with the other vertices.");
      return D_ERROR;
    }
  }

  u64 vertex_buffer_size = vertex->size * darray_size(&vertex_buffer_info->vertices);

  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_memory;

  //TODO:
  // create_buffer(vertex_buffer_size, )

  void *data;
  vkMapMemory(backend->device.logical_device, staging_buffer_memory, 0, vertex_buffer_size, 0, &data);
  memcpy(data, darray_data(&vertex_buffer_info->vertices), (size_t)vertex_buffer_size);
  vkUnmapMemory(backend->device.logical_device, staging_buffer_memory);

  //TODO:
  // create_buffer(...vertex_buffer->buffer_inner)
  // copy_buffer(stagin_buffer, vertex_buffer->buffer_inner, vertex_buffer_size);

  vkDestroyBuffer(backend->device.logical_device, staging_buffer, backend->vulkan_context.allocator);
  vkFreeMemory(backend->device.logical_device, staging_buffer_memory, backend->vulkan_context.allocator);
  
  return D_SUCCESS;
}

DResult render_backend_destroy_vertex_buffer(struct RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer)
{
  return D_SUCCESS;
}