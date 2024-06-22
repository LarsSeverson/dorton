#pragma once

#include "./render_backend_vertex_core.h"

typedef struct RenderBackendVertex
{
  VertexType type;
  void *vertex_inner;

  u64 byte_size;

  VkVertexInputBindingDescription (*get_binding_description)(u32 binding, VertexInputRate input_rate);
  VkVertexInputAttributeDescription *(*get_attribute_descriptions)(u32 binding);
} RenderBackendVertex;

DResult render_backend_create_vertex(RenderBackendVertex *vertex, VertexType vertex_type, void *vertex_data);
DResult render_backend_destroy_vertex(RenderBackendVertex *vertex);
