#pragma once

#include "render/render_backend/render_backend_vertex/render_backend_vertex_core.h"

typedef struct RenderBackendVertex3D
{
  vec3 position;
  vec3 color;

  VertexInputBindingDescription (*get_binding_description)(u32 binding, VertexInputRate input_rate);
  VertexInputAttributeDescription *(*get_attribute_descriptions)(u32 binding);
} RenderBackendVertex3D;

RenderBackendVertex3D vertex_3D_create();