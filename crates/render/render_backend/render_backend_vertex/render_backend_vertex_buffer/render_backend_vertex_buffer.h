#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_vertex/render_backend_vertices.h"
#include "render/render_backend/render_backend_buffer/render_backend_buffer.h"

typedef struct VertexBufferInfo
{
  RenderBackendVertices vertices;

  VertexInputRate input_rate;
  
  u32 binding;
  u32 offset;
  VkMemoryMapFlags flags;
  VkSharingMode sharing_mode;
} VertexBufferInfo;

typedef struct RenderBackendVertexBuffer
{
  RenderBackendBuffer buffer;

  VertexType type;
  RenderBackendVertices vertices;

  u32 binding;
  VertexInputRate input_rate;
} RenderBackendVertexBuffer;

struct RenderBackend;

DResult render_backend_create_vertex_buffer(struct RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer, VertexBufferInfo *vertex_buffer_info);
DResult render_backend_destroy_vertex_buffer(struct RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer);

VkVertexInputBindingDescription render_backend_vertex_buffer_get_binding_description(RenderBackendVertexBuffer *vertex_buffer);
VkVertexInputAttributeDescription *render_backend_vertex_buffer_get_attribute_descriptions(RenderBackendVertexBuffer *vertex_buffer);
