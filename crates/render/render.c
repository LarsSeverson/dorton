#include "render.h"

#include "./render_backend/render_backend.h"

#include "../logger/logger.h"

DResult renderer_create(Renderer *renderer, RendererCreateInfo *create_info)
{
  RenderBackendCreateInfo render_backend_create_info;
  render_backend_create_info.app_title = create_info->app_title;
  render_backend_create_info.window = create_info->window;
  
  if (render_backend_create(&renderer->backend, &render_backend_create_info) != D_SUCCESS)
  {
    DFATAL("Renderer failed to create backend.");
    return D_ERROR;
  }

  DINFO("Renderer created successfully.");

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

DResult renderer_begin_frame(Renderer *renderer, f32 delta_time)
{
  return render_backend_begin_frame(&renderer->backend, delta_time);
}

DResult renderer_end_frame(Renderer *renderer, f32 delta_time)
{
  return render_backend_end_frame(&renderer->backend, delta_time);
}

DResult renderer_draw(Renderer *renderer, RenderPacket packet)
{
  if (renderer_begin_frame(renderer, packet.delta_time) != D_SUCCESS)
  {
    DERROR("Renderer unable to begin frame.");
    return D_ERROR;
  }
  if (renderer_end_frame(renderer, packet.delta_time) != D_SUCCESS)
  {
    DERROR("Renderer unable to end frame.");
    return D_ERROR;
  }

  return D_SUCCESS;
}