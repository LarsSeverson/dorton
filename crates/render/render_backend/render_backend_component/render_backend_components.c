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
  }

  darray_destroy(&components->components);

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

  RenderBackendShaders shaders = {0};
  if (render_backend_create_default_shaders(backend, &shaders) != D_SUCCESS)
  {
    return D_ERROR;
  }

  RenderBackendComponentInfo component_info = {0};
  component_info.render_pass = &render_pass;
  component_info.command_pool = &command_pool;
  component_info.command_buffers = &command_buffers;
  component_info.vertex_buffer = &vertex_buffer;
  component_info.index_buffer = &index_buffer;
  component_info.shaders = &shaders;

  u32 components_size = components->components_size;

  if (render_backend_components_push(backend, components, &component_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  RenderBackendComponent *component = (RenderBackendComponent *)darray_get(&components->components, components_size);

  VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

  RenderBackendRasterizerInfo rasterizer_info = render_backend_create_default_rasterizer_info();
  RenderBackendMultisampleInfo multisample_info = render_backend_create_default_multisample_info();
  RenderBackendColorBlendInfo color_blend_info = render_backend_create_default_color_blend_info();

  VkViewport viewport = {};
  viewport.x = 0.f;
  viewport.y = 0.f;
  viewport.width = (f32)backend->swap_chain.extent.width;
  viewport.height = (f32)backend->swap_chain.extent.height;
  viewport.minDepth = 0.f;
  viewport.maxDepth = 1.f;

  // 2D offset problems when working with 3D?
  VkScissor scissor = {};
  scissor.offset = (VkOffset2D){0, 0};
  scissor.extent = backend->swap_chain.extent;

  DArray dynamic_states;
  darray_reserve(&dynamic_states, VkDynamicState, 2);
  darray_set(&dynamic_states, VK_DYNAMIC_STATE_VIEWPORT, 0);
  darray_set(&dynamic_states, VK_DYNAMIC_STATE_SCISSOR, 1);

  RenderBackendPipelineLayout pipeline_layout = render_backend_create_default_pipeline_layout();

  ComponentPipelineInfo component_pipeline_info = {0};
  component_pipeline_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  component_pipeline_info.viewport = viewport;
  component_pipeline_info.scissor = scissor;
  component_pipeline_info.rasterizer_info = &rasterizer_info;
  component_pipeline_info.multisample_info = &multisample_info;
  component_pipeline_info.color_blend_info = &color_blend_info;
  component_pipeline_info.dynamic_states = &dynamic_states;
  component_pipeline_info.pipeline_layout = &pipeline_layout;

  if (render_backend_component_create_pipeline(backend, component, &component_pipeline_info) != D_SUCCESS)
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
    return D_ERROR;
  }

  for (u32 i = 0; i < size; ++i)
  {
    RenderBackendComponent *component = (RenderBackendComponent *)darray_get(&components->components, i);

    if (render_backend_process_component(backend, component, draw_packet) != D_SUCCESS)
    {
      DERROR("Could not process components.");
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