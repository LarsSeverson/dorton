#include "render.h"

#include "logger/logger.h"

#include "./render_backend/render_backend.h"

DResult renderer_create(Renderer *renderer, RendererCreateInfo *create_info)
{
  RenderBackendCreateInfo render_backend_create_info;
  render_backend_create_info.app_title = create_info->app_title;
  render_backend_create_info.window = create_info->window;

  if (render_backend_create(&renderer->backend, &render_backend_create_info) != D_SUCCESS)
  {
    DFATAL("Renderer failed to create backend.");
    return D_FATAL;
  }

  DINFO("Renderer created successfully.");

  render_backend_add_empty_component(&renderer->backend);
  RenderBackendComponent *component = (RenderBackendComponent*)darray_get(&renderer->backend.components.components, 0);

  RenderBackendVertex2D vertex1 = {{-0.5f, -0.5f}, {1.f, 0.f, 0.f}};

  RenderBackendVertex2D vertex2 = {{0.5f, -0.5f}, {0.f, 1.f, 0.f}};

  RenderBackendVertex2D vertex3 = {{0.5f, 0.5f}, {0.f, 0.f, 1.f}};

  RenderBackendVertex2D vertex4 = {{-0.5f, 0.5f}, {1.f, 1.f, 1.f}};

  RenderBackendVertices vertices;
  render_backend_create_vertices(&vertices, VERTEX_TYPE_2D);
  
  render_backend_vertices_push(&vertices, &vertex1);
  render_backend_vertices_push(&vertices, &vertex2);
  render_backend_vertices_push(&vertices, &vertex3);
  render_backend_vertices_push(&vertices, &vertex4);

  RenderBackendIndex indices_data[] = {0, 1, 2, 2, 3, 0};
  RenderBackendIndices indices;
  render_backend_create_indices(&indices, indices_data, sizeof(indices_data) / sizeof(indices_data[0]));

  render_backend_component_set_vertices(&renderer->backend, component, &vertices);
  render_backend_component_set_indices(&renderer->backend, component, &indices);

  return D_SUCCESS;
}

DResult renderer_destroy(Renderer *renderer)
{
  if (render_backend_destroy(&renderer->backend) != D_SUCCESS)
  {
    DERROR("Renderer faied to destroy backend.");
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult renderer_draw(Renderer *renderer, RenderPacket render_packet)
{
  if (render_backend_draw(&renderer->backend, render_packet) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult renderer_resize(Renderer *renderer, i32 width, i32 height)
{
  if (render_backend_resize(&renderer->backend, width, height) != D_SUCCESS)
  {
    return D_FATAL;
  }

  return D_SUCCESS;
}