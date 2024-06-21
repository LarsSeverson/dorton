#pragma once

#include "render/render_backend/render_backend_vertex/render_backend_vertex_core.h"

typedef struct RenderBackendVertex3D
{
  vec3 position;
  vec3 color;

} RenderBackendVertex3D;

struct RenderBackendVertex;

RenderBackendVertex3D *render_backend_create_vertex_3D(struct RenderBackendVertex *vertex, void *vertex_data);
DResult render_backend_destroy_vertex_3D(struct RenderBackendVertex *vertex);