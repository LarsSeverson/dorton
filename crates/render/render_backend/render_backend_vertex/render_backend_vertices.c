#include "render_backend_vertices.h"

#include "logger.h"

DResult render_backend_create_vertices(RenderBackendVertices *vertices, VertexType vertices_type)
{
  darray_create(&vertices->vertices_inner, RenderBackendVertex);
  vertices->type = vertices_type;

  return D_SUCCESS;
}

DResult render_backend_destroy_vertices(RenderBackendVertices *vertices)
{
  for (u32 i = 0; i < darray_size(&vertices->vertices_inner); ++i)
  {
    RenderBackendVertex *vertex = (RenderBackendVertex *)darray_get(&vertices->vertices_inner, i);
    render_backend_destroy_vertex(vertex);
  }

  darray_destroy(&vertices->vertices_inner);
  *vertices = (RenderBackendVertices){0};

  return D_SUCCESS;
}

DResult render_backend_vertices_push(RenderBackendVertices *vertices, RenderBackendVertex *vertex)
{
  if (vertex == NULL)
  {
    return D_IGNORED;
  }

  if (vertices->type != vertex->type)
  {
    DERROR("Vertex type mismatch.");
    return D_ERROR;
  }

  darray_push(&vertices->vertices_inner, *vertex);

  return D_SUCCESS;
}

DResult render_backend_vertices_push_raw(RenderBackendVertices *vertices, VertexType vertex_type, void *vertex_data)
{
  RenderBackendVertex new_vertex = {0};
  if (render_backend_create_vertex(&new_vertex, vertex_type, vertex_data) != D_SUCCESS)
  {
    return D_ERROR;
  }

  if (render_backend_vertices_push(vertices, &new_vertex) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

RenderBackendVertex *render_backend_vertices_get(RenderBackendVertices *vertices, u32 index)
{
  return (RenderBackendVertex *)darray_get(&vertices->vertices_inner, index);
}

RenderBackendVertex *render_backend_vertices_data(RenderBackendVertices *vertices)
{
  return (RenderBackendVertex *)darray_data(&vertices->vertices_inner);
}

u32 render_backend_vertices_size(RenderBackendVertices *vertices)
{
  return darray_size(&vertices->vertices_inner);
}

VkVertexInputBindingDescription render_backend_vertices_get_binding_description(RenderBackendVertices *vertices, u32 binding, VertexInputRate input_rate)
{
  return render_backend_vertices_get(vertices, 0)->get_binding_description(binding, input_rate);
}

DArray render_backend_vertices_get_attribute_descriptions(RenderBackendVertices *vertices, u32 binding)
{
  return render_backend_vertices_get(vertices, 0)->get_attribute_descriptions(binding);
}
