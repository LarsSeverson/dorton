#pragma once

#include "./render_backend_vertex_core.h"
#include "./render_backend_vertex_2D/render_backend_vertex_2D.h"
#include "./render_backend_vertex_3D/render_backend_vertex_3D.h"

struct RenderBackend;

typedef struct Vertex
{
  VertexType type;
  union {
    RenderBackendVertex2D vertex_2D;
    RenderBackendVertex3D vertex_3D;
  } data;

  u64 size;
  
  VertexInputBindingDescription (*get_binding_description)(u32 binding, VertexInputRate input_rate);
  VertexInputAttributeDescription *(*get_attribute_descriptions)(u32 binding);
} Vertex;

DResult vertex_create(Vertex *vertex, VertexType vertex_type);
DResult vertex_destroy(Vertex *vertex);

