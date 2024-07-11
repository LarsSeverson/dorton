#include "render_backend_vertices.h"

#include "logger.h"

DResult vertices_push_2D(RenderBackendVertices *vertices, void *vertex);
DResult vertices_push_3D(RenderBackendVertices *vertices, void *vertex);

DResult render_backend_create_vertices(RenderBackendVertices *vertices, VertexType vertices_type)
{
  *vertices = (RenderBackendVertices){0};

  if (vertices_type == VERTEX_TYPE_2D)
  {
    darray_create(&vertices->vertices_inner, RenderBackendVertex2D);

    vertices->push = vertices_push_2D;

    vertices->get_binding_description = get_vertex_2D_binding_description;
    vertices->get_attribute_descriptions = get_vertex_2D_attribute_descriptions;
  }
  else
  {
    darray_create(&vertices->vertices_inner, RenderBackendVertex3D);

    // TODO:
    vertices->push = vertices_push_3D;
  }

  vertices->type = vertices_type;

  return D_SUCCESS;
}

DResult render_backend_destroy_vertices(RenderBackendVertices *vertices)
{
  darray_destroy(&vertices->vertices_inner);

  *vertices = (RenderBackendVertices){0};

  return D_SUCCESS;
}

DResult render_backend_vertices_push(RenderBackendVertices *vertices, void *vertex)
{
  return vertices->push(vertices, vertex);
}

void *render_backend_vertices_get(RenderBackendVertices *vertices, u64 index)
{
  return darray_get(&vertices->vertices_inner, index);
}

void *render_backend_vertices_data(RenderBackendVertices *vertices)
{
  return darray_data(&vertices->vertices_inner);
}

u64 render_backend_vertices_size(RenderBackendVertices *vertices)
{
  return vertices->size;
}

u64 render_backend_vertices_byte_size(RenderBackendVertices *vertices)
{
  return vertices->byte_size;
}

DResult vertices_push_2D(RenderBackendVertices *vertices, void *vertex)
{
  RenderBackendVertex2D new_vertex = *(RenderBackendVertex2D *)vertex;
  darray_push(&vertices->vertices_inner, new_vertex);

  vertices->size = darray_size(&vertices->vertices_inner);
  vertices->byte_size = vertices->size * sizeof(RenderBackendVertex2D);

  return D_SUCCESS;
}

DResult vertices_push_3D(RenderBackendVertices *vertices, void *vertex)
{
  RenderBackendVertex3D new_vertex = *(RenderBackendVertex3D *)vertex;
  darray_push(&vertices->vertices_inner, new_vertex);

  vertices->size = darray_size(&vertices->vertices_inner);
  vertices->byte_size = vertices->size * sizeof(RenderBackendVertex3D);

  return D_SUCCESS;
}