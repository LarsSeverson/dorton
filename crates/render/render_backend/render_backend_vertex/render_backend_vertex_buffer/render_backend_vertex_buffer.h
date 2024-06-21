#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_vertex/render_backend_vertices.h"
#include "render/render_backend/render_backend_buffer/render_backend_buffer.h"

typedef struct VertexBufferInfo
{
  RenderBackendVertices vertices;
  
  u32 binding;
  u32 offset;
  VkMemoryMapFlags flags;
} VertexBufferInfo;

typedef struct RenderBackendVertexBuffer
{
  RenderBackendBuffer buffer;

  VertexType type;
  RenderBackendVertices vertices;

  u32 binding;
} RenderBackendVertexBuffer;

struct RenderBackend;

DResult render_backend_create_vertex_buffer(struct RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer, VertexBufferInfo *vertex_buffer_info);
DResult render_backend_destroy_vertex_buffer(struct RenderBackend *backend, RenderBackendVertexBuffer *vertex_buffer);
