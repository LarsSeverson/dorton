#include "render_backend_vertex_buffer.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"
#include "render/render_backend/render_backend_buffer/render_backend_buffer.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex_2D/render_backend_vertex_2D.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex_3D/render_backend_vertex_3D.h"

#include <string.h>

DResult render_backend_create_vertex_buffer(RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer, VertexBufferInfo *vertex_buffer_info)
{
  RenderBackendVertex *vertex = render_backend_vertices_get(&vertex_buffer_info->vertices, 0);
  if (vertex == NULL)
  {
    DERROR("Could not determine the vertex buffer type.");
    return D_ERROR;
  }

  VertexType type = vertex->type;
  u64 size = render_backend_vertices_size(&vertex_buffer_info->vertices);

  for (u32 i = 1; i < size; ++i)
  {
    RenderBackendVertex *vertex = render_backend_vertices_get(&vertex_buffer_info->vertices, i);
    if (vertex->type != type)
    {
      DERROR("Vertex %p has conflicting type with the other vertices.", vertex);
      return D_ERROR;
    }
  }

  u64 vertex_buffer_size = vertex->byte_size * size;

  BufferInfo staging_buffer_info = {VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
  staging_buffer_info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  staging_buffer_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
  staging_buffer_info.size = vertex_buffer_size;
  staging_buffer_info.offset = 0;

  RenderBackendBuffer staging_buffer;
  if (render_backend_create_buffer(backend, &staging_buffer, staging_buffer_info) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  RenderBackendVertex *vertices_data = render_backend_vertices_data(&vertex_buffer_info->vertices);

  void *data;
  vkMapMemory(backend->device.logical_device, staging_buffer.memory, vertex_buffer_info->offset, vertex_buffer_size, vertex_buffer_info->flags, &data);
  memcpy(data, vertices_data, (size_t)vertex_buffer_size);
  vkUnmapMemory(backend->device.logical_device, staging_buffer.memory);

  BufferInfo vertex_buffer_create_info = {VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
  vertex_buffer_create_info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  vertex_buffer_create_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
  vertex_buffer_create_info.size = vertex_buffer_size;
  vertex_buffer_create_info.offset = 0;

  if (render_backend_create_buffer(backend, &vertex_buffer->buffer, vertex_buffer_create_info) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  if (render_backend_copy_buffer(backend, &staging_buffer, &vertex_buffer->buffer, vertex_buffer_size) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  vkDestroyBuffer(backend->device.logical_device, staging_buffer.buffer_inner, backend->vulkan_context.allocator);
  vkFreeMemory(backend->device.logical_device, staging_buffer.memory, backend->vulkan_context.allocator);

  vertex_buffer->vertices = vertex_buffer_info->vertices;
  vertex_buffer->binding = vertex_buffer_info->binding;
  vertex_buffer->type = type;

  return D_SUCCESS;
}

DResult render_backend_destroy_vertex_buffer(RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer)
{
  render_backend_destroy_buffer(backend, &vertex_buffer->buffer);
  render_backend_destroy_vertices(&vertex_buffer->vertices);

  *vertex_buffer = (RenderBackendVertexBuffer){0};

  return D_SUCCESS;
}