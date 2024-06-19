#include "render_backend_vertex.h"
#include "logger.h"

DResult vertex_create(Vertex *vertex, VertexType vertex_type)
{
  if (vertex_type > VERTEX_TYPE_3D)
  {
    DERROR("Unknown vertex type.");
    return D_ERROR;
  }

  if (vertex_type == VERTEX_TYPE_2D)
  {
    vertex->type = VERTEX_TYPE_2D;
    vertex->data.vertex_2D = vertex_2D_create();
    vertex->size = sizeof(RenderBackendVertex2D);
    vertex->get_binding_description = vertex->data.vertex_2D.get_binding_description;
    vertex->get_attribute_descriptions = vertex->data.vertex_2D.get_attribute_descriptions;
  }

  return D_SUCCESS;
}

DResult vertex_destroy(Vertex *vertex)
{
  return D_SUCCESS;
}
