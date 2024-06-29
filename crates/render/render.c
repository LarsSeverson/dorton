#include "render.h"

#include "./render_backend/render_backend.h"

#include "../logger/logger.h"

#include "./render_backend/render_backend_pipeline/render_backend_pipeline.h"

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

  // Test
  RenderBackendVertex vertex1;
  RenderBackendVertex2D vertex_data1 = {{-0.5f, -0.5f}, {1.f, 0.f, 0.f}};
  render_backend_create_vertex(&vertex1, VERTEX_TYPE_2D, &vertex_data1);

  RenderBackendVertex vertex2;
  RenderBackendVertex2D vertex_data2 = {{0.5f, -0.5f}, {0.f, 1.f, 0.f}};
  render_backend_create_vertex(&vertex2, VERTEX_TYPE_2D, &vertex_data2);

  RenderBackendVertex vertex3;
  RenderBackendVertex2D vertex_data3 = {{0.5f, 0.5f}, {0.f, 1.f, 0.f}};
  render_backend_create_vertex(&vertex3, VERTEX_TYPE_2D, &vertex_data3);

  RenderBackendVertices vertices;
  render_backend_create_vertices(&vertices, VERTEX_TYPE_2D);
  render_backend_vertices_push(&vertices, &vertex1);
  render_backend_vertices_push(&vertices, &vertex2);
  render_backend_vertices_push(&vertices, &vertex3);

  RenderBackendIndices indices;
  RenderBackendIndex indices_data[3] = {0, 1, 2};
  render_backend_create_indices(&indices, indices_data, 3);

  VertexBufferInfo vertex_buffer_info = {0};
  vertex_buffer_info.vertices = vertices;

  RenderBackendVertexBuffer vertex_buffer;
  render_backend_create_vertex_buffer(&renderer->backend, &vertex_buffer, &vertex_buffer_info);

  ShaderInfo vert_shader_info = {0};
  vert_shader_info.main = "main";
  vert_shader_info.src = "test";
  vert_shader_info.type = SHADER_TYPE_VERTEX;

  RenderBackendShader vert_shader;

  ShaderInfo frag_shader_info = {0};
  frag_shader_info.main = "main";
  frag_shader_info.src = "test";
  frag_shader_info.type = SHADER_TYPE_FRAGMENT;

  RenderBackendShader frag_shader;

  RenderBackendShaders shaders;
  render_backend_create_shaders(&renderer->backend, &shaders);
  render_backend_shaders_push(&renderer->backend, &shaders, &vert_shader, &vert_shader_info);
  render_backend_shaders_push(&renderer->backend, &shaders, &frag_shader, &frag_shader_info);

  VkVertexInputBindingDescription binding_description = render_backend_vertex_buffer_get_binding_description(&vertex_buffer);
  DArray attribute_descriptions = render_backend_vertex_buffer_get_attribute_descriptions(&vertex_buffer);
  DArray binding_descriptions;
  darray_reserve(&binding_descriptions, VkVertexInputBindingDescription, 1);
  darray_set(&binding_descriptions, binding_description, 0);

  VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

  RenderBackendRasterizerInfo rast_info = render_backend_create_default_rasterizer_info();
  RenderBackendMultisampleInfo mul_info = render_backend_create_default_multisample_info();
  RenderBackendColorBlendInfo color_info = render_backend_create_default_color_blend_info();

  DArray dynamic_states;
  darray_reserve(&dynamic_states, VkDynamicState, 2);
  darray_set(&dynamic_states, VK_DYNAMIC_STATE_VIEWPORT, 0);
  darray_set(&dynamic_states, VK_DYNAMIC_STATE_SCISSOR, 1);

  RenderBackendPipelineLayout pipe_layout = render_backend_create_default_pipeline_layout();

  PipelineInfo pi = {0};
  pi.shaders = shaders;
  pi.attribute_descriptions = attribute_descriptions;
  pi.binding_descriptions = binding_descriptions;
  pi.topology = topology;
  pi.rasterizer_info = rast_info;
  pi.multisample_info = mul_info;
  pi.color_blend_info = color_info;
  pi.dynamic_states = dynamic_states;
  pi.pipeline_layout = pipe_layout;
  pi.render_pass = renderer->backend.render_pass;

  RenderBackendPipeline pipeline;
  render_backend_create_pipeline(&renderer->backend, &pipeline, &pi);

  render_backend_destroy_pipeline(&renderer->backend, &pipeline);
  render_backend_destroy_vertex_buffer(&renderer->backend, &vertex_buffer);
  render_backend_destroy_shaders(&renderer->backend, &shaders);
  // Test

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

DResult renderer_resize(Renderer *renderer, i32 width, i32 height)
{
  if (render_backend_resize(&renderer->backend, width, height) != D_SUCCESS)
  {
    return D_FATAL;
  }

  return D_SUCCESS;
}