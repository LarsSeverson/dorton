#include "render_backend_component.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_component(RenderBackend *backend, RenderBackendComponent *component, RenderBackendComponentInfo *component_info)
{
  DINFO("Creating backend component:");

  *component = (RenderBackendComponent){0};

  component->render_pass = *component_info->render_pass;

  FramebuffersInfo framebuffers_info = {0};
  framebuffers_info.render_pass = &component->render_pass;

  if (render_backend_create_framebuffers(backend, &component->framebuffers, &framebuffers_info) != D_SUCCESS)
  {
    DERROR("\tCould not create component.");
    return D_ERROR;
  }

  component->command_pool = *component_info->command_pool;
  component->command_buffers = *component_info->command_buffers;
  component->vertex_buffer = *component_info->vertex_buffer;
  component->index_buffer = *component_info->index_buffer;

  return D_SUCCESS;
}

DResult render_backend_destroy_component(RenderBackend *backend, RenderBackendComponent *component)
{

  render_backend_destroy_pipeline(backend, &component->pipeline);
  render_backend_destroy_index_buffer(backend, &component->index_buffer);
  render_backend_destroy_vertex_buffer(backend, &component->vertex_buffer);
  render_backend_destroy_command_buffers(backend, &component->command_buffers);
  render_backend_destroy_framebuffers(backend, &component->framebuffers);
  render_backend_destroy_render_pass(backend, &component->render_pass);

  return D_SUCCESS;
}

DResult render_backend_process_component(RenderBackend *backend, RenderBackendComponent *component, RenderBackendDrawPacket *draw_packet)
{
  CommandBuffersProcessInfo process_info = {0};
  process_info.pipeline = &component->pipeline;
  process_info.framebuffer = framebuffers_get(&component->framebuffers, draw_packet->image_index);
  process_info.vertex_buffer = &component->vertex_buffer;
  process_info.index_buffer = &component->index_buffer;
  process_info.start = draw_packet->current_frame;
  process_info.count = 1;

  if (render_backend_process_command_buffers(backend, &component->command_buffers, &process_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  darray_push(&draw_packet->draw_command_buffers, process_info.command_buffer->command_buffer_inner);

  return D_SUCCESS;
}

DResult render_backend_recreate_component_framebuffers(RenderBackend *backend, RenderBackendComponent *component)
{
  return render_backend_recreate_framebuffers(backend, &component->framebuffers);
}