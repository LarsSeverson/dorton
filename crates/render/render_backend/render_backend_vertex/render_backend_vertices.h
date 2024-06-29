#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_vertex/render_backend_vertex.h"

typedef struct RenderBackendVertices
{
  VertexType type;
  // RenderBackendVertex
  DArray vertices_inner;

} RenderBackendVertices;

DResult render_backend_create_vertices(RenderBackendVertices *vertices, VertexType vertices_type);
DResult render_backend_destroy_vertices(RenderBackendVertices *vertices);

DResult render_backend_vertices_push(RenderBackendVertices *vertices, RenderBackendVertex *vertex);
DResult render_backend_vertices_push_raw(RenderBackendVertices *vertices, VertexType vertex_type, void *vertex_data);

RenderBackendVertex *render_backend_vertices_get(RenderBackendVertices *vertices, u32 index);
RenderBackendVertex *render_backend_vertices_data(RenderBackendVertices *vertices);

u32 render_backend_vertices_size(RenderBackendVertices *vertices);

VkVertexInputBindingDescription render_backend_vertices_get_binding_description(RenderBackendVertices *vertices, u32 binding, VertexInputRate input_rate);
DArray render_backend_vertices_get_attribute_descriptions(RenderBackendVertices *vertices, u32 binding);