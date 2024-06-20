#pragma once

#include "render/render_backend/render_backend_vertex/render_backend_vertex_core.h"

typedef struct RenderBackendVertex2D
{
  vec2 position;
  vec3 color;

  VertexInputBindingDescription (*get_binding_description)(u32 binding, VertexInputRate input_rate);
  VertexInputAttributeDescription *(*get_attribute_descriptions)(u32 binding);
} RenderBackendVertex2D;

RenderBackendVertex2D vertex_2D_create();
