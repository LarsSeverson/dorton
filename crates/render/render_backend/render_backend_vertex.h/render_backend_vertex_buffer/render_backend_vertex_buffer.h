#pragma once

#include "../render_backend_vertex_core.h"

#include "darray/darray.h"

typedef struct VertexBufferInfo
{
  DArray vertices;
  
  u32 binding;
} VertexBufferInfo;

typedef struct RenderBackendVertexBuffer
{
  VkBuffer buffer_inner;
  VkDeviceMemory buffer_memory;

  // Vertex
  DArray vertices;
  VertexType type;

  u32 binding;
} RenderBackendVertexBuffer;

struct RenderBackend;

DResult render_backend_create_vertex_buffer(struct RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer, VertexBufferInfo *vertex_buffer_info);
DResult render_backend_destroy_vertex_buffer(struct RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer);
