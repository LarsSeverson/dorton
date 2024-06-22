#pragma once

#include "render/render_types.h"
#include "darray/darray.h"

#include "./render_backend_pipeline_core.h"

typedef struct PipelineInfo
{
  VkViewport viewport;
  VkRect2D scissor;

  ShaderFlagBits shader_flags;
  RenderBackendShaders shaders;

  CullModeFlagBits cull_flags;

  // VkVertexInputBindingDescription
  DArray binding_descriptions;
  // VkVertexInputAttributeDescription
  DArray attribute_descriptions;

} PipelineInfo;

typedef struct RenderBackendPipeline
{

} RenderBackendPipeline;

struct RenderBackend;

DResult render_backend_create_pipeline(struct RenderBackend *backend, RenderBackendPipeline *pipeline, PipelineInfo *pipeline_info);