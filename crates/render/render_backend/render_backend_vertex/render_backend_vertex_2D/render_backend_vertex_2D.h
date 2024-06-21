#pragma once

#include "render/render_backend/render_backend_vertex/render_backend_vertex_core.h"

typedef struct RenderBackendVertex2D
{
  vec2 position;
  vec3 color;

} RenderBackendVertex2D;

struct RenderBackendVertex;

RenderBackendVertex2D *render_backend_create_vertex_2D(struct RenderBackendVertex *vertex, void *vertex_data);