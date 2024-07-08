#include "render_backend_components.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_components(RenderBackend *backend, RenderBackendComponents *components)
{
  *components = (RenderBackendComponents){0};

  darray_create(&components->components, RenderBackendComponent);

  return D_SUCCESS;
}

DResult render_backend_destroy_components(RenderBackend *backend, RenderBackendComponents *components)
{

  u32 size = components->components_size;

  if (size)
  {
    for (u32 i = 0; i < size; ++i)
    {
      RenderBackendComponent *component = (RenderBackendComponent *)darray_get(&components->components, i);
      render_backend_destroy_component(backend, component);
    }

    darray_destroy(&components->components);
  }

  *components = (RenderBackendComponents){0};

  return D_SUCCESS;
}

DResult render_backend_components_push(RenderBackend *backend, RenderBackendComponents *components, RenderBackendComponentInfo *component_info)
{
  RenderBackendComponent component = {0};

  if (render_backend_create_component(backend, &component, component_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  darray_push(&components->components, component);
  components->components_size = (u32)darray_size(&components->components);

  return D_SUCCESS;
}

DResult render_backend_components_push_empty(RenderBackend *backend, RenderBackendComponents *components)
{
  RenderPassInfo render_pass_info = create_default_render_pass_info(backend);
  RenderBackendRenderPass render_pass = {0};
  if (render_backend_create_render_pass(backend, &render_pass, &render_pass_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  CommandPoolInfo command_pool_info = create_default_command_pool_info(backend);
  RenderBackendCommandPool command_pool = {0};
  if (render_backend_create_command_pool(backend, &command_pool, &command_pool_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  CommandBuffersInfo command_buffers_info = create_default_command_buffers_info(backend);
  command_buffers_info.command_pool = &command_pool;

  RenderBackendCommandBuffers command_buffers = {0};
  if (render_backend_create_command_buffers(backend, &command_buffers, &command_buffers_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  RenderBackendVertexBuffer vertex_buffer = {0};
  if (render_backend_create_empty_vertex_buffer(backend, &vertex_buffer) != D_SUCCESS)
  {
    return D_ERROR;
  }

  RenderBackendIndexBuffer index_buffer = {0};
  if (render_backend_create_empty_index_buffer(backend, &index_buffer) != D_SUCCESS)
  {
    return D_ERROR;
  }

  RenderBackendComponentInfo component_info = {0};
  component_info.render_pass = &render_pass;
  component_info.command_pool = &command_pool;
  component_info.command_buffers = &command_buffers;
  component_info.vertex_buffer = &vertex_buffer;
  component_info.index_buffer = &index_buffer;

  if (render_backend_components_push(backend, components, &component_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  DINFO("[Render Backend]: Added empty component.");

  return D_SUCCESS;
}

DResult render_backend_process_components(RenderBackend *backend, RenderBackendComponents *components, RenderBackendDrawPacket *draw_packet)
{
  u32 size = components->components_size;

  if (size == 0)
  {
    DINFO("Cannot process components. Components' size was 0.");
    return D_IGNORED;
  }

  for (u32 i = 0; i < size; ++i)
  {
    RenderBackendComponent *component = (RenderBackendComponent *)darray_get(&components->components, i);

    if (render_backend_process_component(backend, component, draw_packet) != D_SUCCESS)
    {
      return D_ERROR;
    }
  }

  return D_SUCCESS;
}

DResult render_backend_components_recreate_framebuffers(RenderBackend *backend, RenderBackendComponents *components)
{
  u32 size = components->components_size;

  for (u32 i = 0; i < size; ++i)
  {
    RenderBackendComponent *component = (RenderBackendComponent *)darray_get(&components->components, i);
    if (render_backend_recreate_component_framebuffers(backend, component) != D_SUCCESS)
    {
      return D_ERROR;
    }
  }

  return D_SUCCESS;
}