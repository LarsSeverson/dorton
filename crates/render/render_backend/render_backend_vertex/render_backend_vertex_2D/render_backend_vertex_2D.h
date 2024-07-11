#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_vertex/render_backend_vertex_core.h"

typedef struct RenderBackendVertex2D
{
  vec2 position;
  vec3 color;

} RenderBackendVertex2D;

VkVertexInputBindingDescription get_vertex_2D_binding_description(u32 binding, VertexInputRate input_rate);
DArray get_vertex_2D_attribute_descriptions(u32 binding);
