#pragma once

#include "darray/darray.h"

#include "./render_backend_pipeline_core.h"
#include "./render_backend_pipeline_rasterizer/render_backend_pipeline_rasterizer.h"
#include "./render_backend_pipeline_multisample/render_backend_pipeline_multisample.h"
#include "./render_backend_pipeline_color_blend/render_backend_pipeline_color_blend.h"
#include "./render_backend_pipeline_layout/render_backend_pipeline_layout.h"

#include "render/render_backend/render_backend_shader/render_backend_shader_lib.h"
#include "render/render_backend/render_backend_render_pass/render_backend_render_pass.h"

typedef VkRect2D VkScissor;

typedef struct PipelineInfo
{
  ShaderFlagBits shader_flags;
  RenderBackendShaders shaders;

  // VkVertexInputBindingDescription
  DArray binding_descriptions;
  // VkVertexInputAttributeDescription
  DArray attribute_descriptions;

  VkPrimitiveTopology topology;

  VkViewport viewport; // TODO: Multiple viewports
  VkScissor scissor;   // TODO: Multiple scissors

  RenderBackendRasterizerInfo rasterizer_info;
  RenderBackendMultisampleInfo multisample_info;
  RenderBackendColorBlendInfo color_blend_info;

  // VkDynamicState
  DArray dynamic_states;

  RenderBackendPipelineLayout pipeline_layout;

  RenderBackendRenderPass render_pass;

} PipelineInfo;

typedef struct RenderBackendPipeline
{
  VkPipeline pipeline_inner;
  VkPipelineLayout layout;

  RenderBackendRenderPass render_pass;
  VkViewport viewport;
  VkScissor scissor;

} RenderBackendPipeline;

struct RenderBackend;

DResult render_backend_create_pipeline(struct RenderBackend *backend, RenderBackendPipeline *pipeline, PipelineInfo *pipeline_info);
DResult render_backend_destroy_pipeline(struct RenderBackend *backend, RenderBackendPipeline *pipeline);