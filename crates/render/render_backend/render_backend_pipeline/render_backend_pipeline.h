#pragma once

#include "darray/darray.h"

#include "./render_backend_pipeline_core.h"
#include "./render_backend_pipeline_rasterizer/render_backend_rasterizer.h"

#include "render/render_backend/render_backend_shader/render_backend_shader_lib.h"
#include "render/render_backend/render_backend_render_pass/render_backend_render_pass.h"

typedef struct PipelineInfo
{
  ShaderFlagBits shader_flags;
  RenderBackendShaders shaders;

  // VkVertexInputBindingDescription
  DArray binding_descriptions;
  // VkVertexInputAttributeDescription
  DArray attribute_descriptions;

  Topology topology;

  RenderBackendRasterizerInfo rasterizer_info;

  VkViewport viewport; // TODO: Multiple viewports
  VkRect2D scissor;    // TODO: Multiple scissors

  RenderBackendRenderPass render_pass;

} PipelineInfo;

typedef struct RenderBackendPipeline
{

} RenderBackendPipeline;

struct RenderBackend;

DResult render_backend_create_pipeline(struct RenderBackend *backend, RenderBackendPipeline *pipeline, PipelineInfo *pipeline_info);