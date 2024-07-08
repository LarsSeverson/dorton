#include "render_backend_vertex_buffer.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"
#include "render/render_backend/render_backend_buffer/render_backend_buffer.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex_2D/render_backend_vertex_2D.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex_3D/render_backend_vertex_3D.h"

#include <string.h>

#define DEFAULT_VERTEX_BUFFER_SIZE sizeof(RenderBackendVertex3D) * 8 * 1024 * 1024

DResult render_backend_create_empty_vertex_buffer(RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer)
{
  u64 vertex_buffer_size = DEFAULT_VERTEX_BUFFER_SIZE;

  BufferInfo staging_buffer_info = {VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
  staging_buffer_info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  staging_buffer_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
  staging_buffer_info.size = vertex_buffer_size;
  staging_buffer_info.offset = 0;

  RenderBackendBuffer staging_buffer;
  if (render_backend_create_buffer(backend, &staging_buffer, &staging_buffer_info) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  void *data;
  vkMapMemory(backend->device.logical_device, staging_buffer.memory, 0, vertex_buffer_size, 0, &data);
  memcpy(data, 0, (size_t)vertex_buffer_size);
  vkUnmapMemory(backend->device.logical_device, staging_buffer.memory);

  BufferInfo vertex_buffer_create_info = {VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
  vertex_buffer_create_info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  vertex_buffer_create_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
  vertex_buffer_create_info.size = vertex_buffer_size;
  vertex_buffer_create_info.offset = 0;

  if (render_backend_create_buffer(backend, &vertex_buffer->buffer, &vertex_buffer_create_info) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  if (render_backend_copy_buffer(backend, &staging_buffer, &vertex_buffer->buffer, vertex_buffer_size) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  render_backend_destroy_buffer(backend, &staging_buffer);

  vertex_buffer->vertices = (RenderBackendVertices){0};
  vertex_buffer->binding = 0;
  vertex_buffer->type = VERTEX_TYPE_3D;

  return D_SUCCESS;
}

DResult render_backend_create_vertex_buffer(RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer, VertexBufferInfo *vertex_buffer_info)
{
  *vertex_buffer = (RenderBackendVertexBuffer){0};

  if (vertex_buffer_info == NULL)
  {
  }

  RenderBackendVertex *vertex = render_backend_vertices_get(&vertex_buffer_info->vertices, 0);
  if (vertex == NULL)
  {
    DERROR("Could not determine the vertex buffer type.");
    return D_ERROR;
  }

  VertexType type = vertex->type;
  u64 verices_size = render_backend_vertices_size(&vertex_buffer_info->vertices);

  for (u32 i = 1; i < verices_size; ++i)
  {
    RenderBackendVertex *vertex = render_backend_vertices_get(&vertex_buffer_info->vertices, i);
    if (vertex->type != type)
    {
      DERROR("Vertex %p has conflicting type with the other vertices.", vertex);
      return D_ERROR;
    }
  }

  u64 vertex_buffer_size = vertex->byte_size * verices_size;

  BufferInfo staging_buffer_info = {VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
  staging_buffer_info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  staging_buffer_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
  staging_buffer_info.size = vertex_buffer_size;
  staging_buffer_info.offset = 0;

  RenderBackendBuffer staging_buffer;
  if (render_backend_create_buffer(backend, &staging_buffer, &staging_buffer_info) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  RenderBackendVertex *vertices_data = render_backend_vertices_data(&vertex_buffer_info->vertices);

  void *data;
  vkMapMemory(backend->device.logical_device, staging_buffer.memory, 0, vertex_buffer_size, 0, &data);
  memcpy(data, vertices_data, (size_t)vertex_buffer_size);
  vkUnmapMemory(backend->device.logical_device, staging_buffer.memory);

  BufferInfo vertex_buffer_create_info = {VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
  vertex_buffer_create_info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  vertex_buffer_create_info.sharing_mode = vertex_buffer_info->sharing_mode;
  vertex_buffer_create_info.size = vertex_buffer_size;
  vertex_buffer_create_info.offset = vertex_buffer_info->offset;

  if (render_backend_create_buffer(backend, &vertex_buffer->buffer, &vertex_buffer_create_info) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  if (render_backend_copy_buffer(backend, &staging_buffer, &vertex_buffer->buffer, vertex_buffer_size) != D_SUCCESS)
  {
    DERROR("Could not create vertex buffer.");
    return D_ERROR;
  }

  render_backend_destroy_buffer(backend, &staging_buffer);

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

VkVertexInputBindingDescription render_backend_vertex_buffer_get_binding_description(RenderBackendVertexBuffer *vertex_buffer)
{
  return render_backend_vertices_get_binding_description(&vertex_buffer->vertices, vertex_buffer->binding, vertex_buffer->input_rate);
}

DArray render_backend_vertex_buffer_get_attribute_descriptions(RenderBackendVertexBuffer *vertex_buffer)
{
  return render_backend_vertices_get_attribute_descriptions(&vertex_buffer->vertices, vertex_buffer->binding);
}
