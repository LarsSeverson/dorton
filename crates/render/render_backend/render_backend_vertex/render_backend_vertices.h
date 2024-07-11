#pragma once

#include "darray/darray.h"

#include "./render_backend_vertex_2D/render_backend_vertex_2D.h"
#include "./render_backend_vertex_3D/render_backend_vertex_3D.h"

typedef struct RenderBackendVertices
{
  VertexType type;
  // RenderBackendVertex2D or RenderBackendVertex3D depending on type 
  DArray vertices_inner;
  u64 size;
  u64 byte_size;

  DResult (*push)(struct RenderBackendVertices *vertices, void *vertex);

  VkVertexInputBindingDescription (*get_binding_description)(u32 binding, VertexInputRate input_rate);
  DArray (*get_attribute_descriptions)(u32 binding);

} RenderBackendVertices;

DResult render_backend_create_vertices(RenderBackendVertices *vertices, VertexType vertices_type);
DResult render_backend_destroy_vertices(RenderBackendVertices *vertices);

DResult render_backend_vertices_push(RenderBackendVertices *vertices, void *vertex);

void *render_backend_vertices_get(RenderBackendVertices *vertices, u64 index);
void *render_backend_vertices_data(RenderBackendVertices *vertices);

u64 render_backend_vertices_size(RenderBackendVertices *vertices);
u64 render_backend_vertices_byte_size(RenderBackendVertices *vertices);
