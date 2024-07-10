#include "render_backend_component.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_component(RenderBackend *backend, RenderBackendComponent *component, RenderBackendComponentInfo *component_info)
{
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
  component->shaders = *component_info->shaders;

  return D_SUCCESS;
}

DResult render_backend_destroy_component(RenderBackend *backend, RenderBackendComponent *component)
{
  render_backend_destroy_pipeline(backend, &component->pipeline);
  render_backend_destroy_index_buffer(backend, &component->index_buffer);
  render_backend_destroy_vertex_buffer(backend, &component->vertex_buffer);
  render_backend_destroy_command_pool(backend, &component->command_pool);
  render_backend_destroy_command_buffers(backend, &component->command_buffers);
  render_backend_destroy_framebuffers(backend, &component->framebuffers);
  render_backend_destroy_render_pass(backend, &component->render_pass);
  render_backend_destroy_shaders(backend, &component->shaders);

  return D_SUCCESS;
}

DResult render_backend_component_set_vertices(RenderBackendComponent *component, RenderBackendVertices *vertices)
{
  return D_SUCCESS;
}

DResult render_backend_component_set_indices(RenderBackendComponent *component, RenderBackendIndices *indices)
{
  return D_SUCCESS;
}

DResult render_backend_component_create_pipeline(RenderBackend *backend, RenderBackendComponent *component, ComponentPipelineInfo *component_pipeline_info)
{
  PipelineInfo pipeline_info = {0};

  VkVertexInputBindingDescription binding_description;
  DArray attribute_descriptions;

  if (darray_size(&component->vertex_buffer.vertices.vertices_inner))
  {
    binding_description = render_backend_vertex_buffer_get_binding_description(&component->vertex_buffer);

    DArray binding_descriptions;
    darray_reserve(&binding_descriptions, VkVertexInputBindingDescription, 1);
    darray_set(&binding_descriptions, binding_description, 0);

    attribute_descriptions = render_backend_vertex_buffer_get_attribute_descriptions(&component->vertex_buffer);

    pipeline_info.binding_descriptions = &binding_descriptions;
    pipeline_info.attribute_descriptions = &attribute_descriptions;
  }
  else
  {
    pipeline_info.binding_descriptions = NULL;
    pipeline_info.attribute_descriptions = NULL;
  }

  pipeline_info.shader_flags = component_pipeline_info->shader_flags;
  pipeline_info.shaders = &component->shaders;
  pipeline_info.topology = component_pipeline_info->topology;
  pipeline_info.viewport = component_pipeline_info->viewport;
  pipeline_info.scissor = component_pipeline_info->scissor;
  pipeline_info.rasterizer_info = component_pipeline_info->rasterizer_info;
  pipeline_info.multisample_info = component_pipeline_info->multisample_info;
  pipeline_info.color_blend_info = component_pipeline_info->color_blend_info;
  pipeline_info.dynamic_states = component_pipeline_info->dynamic_states;
  pipeline_info.pipeline_layout = component_pipeline_info->pipeline_layout;
  pipeline_info.render_pass = &component->render_pass;

  if (render_backend_create_pipeline(backend, &component->pipeline, &pipeline_info) != D_SUCCESS)
  {
    DERROR("Could not create component pipeline.");
    return D_ERROR;
  }

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
    DERROR("Could not process component.");
    return D_ERROR;
  }

  darray_push(&draw_packet->draw_command_buffers, process_info.command_buffer->command_buffer_inner);

  return D_SUCCESS;
}

DResult render_backend_recreate_component_framebuffers(RenderBackend *backend, RenderBackendComponent *component)
{
  return render_backend_recreate_framebuffers(backend, &component->framebuffers);
}