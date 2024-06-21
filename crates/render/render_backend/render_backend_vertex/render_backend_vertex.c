#include "render_backend_vertex.h"

#include "logger.h"

#include "./render_backend_vertex_2D/render_backend_vertex_2D.h"
#include "./render_backend_vertex_3D/render_backend_vertex_3D.h"

#include <string.h>

DResult render_backend_create_vertex(RenderBackendVertex *vertex, VertexType vertex_type, void *vertex_data)
{
  switch (vertex_type)
  {
  case VERTEX_TYPE_2D:
    vertex->vertex_inner = render_backend_create_vertex_2D(vertex, vertex_data);
    break;

  case VERTEX_TYPE_3D:
    // vertex->vertex_inner = render_backend_create_vertex_3D(vertex, vertex_data);
    break;

  default:
    DERROR("Unknown vertex type.");
    return D_ERROR;
  }

  vertex->type = vertex_type;

  return D_SUCCESS;
}

DResult render_backend_destroy_vertex(RenderBackendVertex *vertex)
{
  free(vertex->vertex_inner);
  *vertex = (RenderBackendVertex){0};
  
  return D_SUCCESS;
}
