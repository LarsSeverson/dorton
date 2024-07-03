#pragma once

#include "darray/darray.h"

#include "./render_backend_core.h"
#include "./render_backend_render_pass/render_backend_render_pass.h"
#include "./render_backend_framebuffer/render_backend_framebuffers.h"
#include "./render_backend_command/render_backend_command_buffer/render_backend_command_buffers.h"
#include "./render_backend_pipeline/render_backend_pipeline.h"

typedef struct RenderBackendComponentInfo
{
  RenderPassInfo *render_pass_info;
  FramebuffersInfo *framebuffer_info;
  CommandBuffersInfo *command_buffers_info;
  PipelineInfo *pipeline_info;

} RenderBackendComponentInfo;

typedef struct RenderBackendComponent
{
  RenderBackendRenderPass render_pass;
  RenderBackendFramebuffers framebuffers;

  // RenderBackendCommandBuffer
  DArray command_buffers;

  RenderBackendPipeline pipeline;

} RenderBackendComponent;

struct RenderBackend;

DResult render_backend_create_component(struct RenderBackend *backend, RenderBackendComponent *component, RenderBackendComponentInfo *component_info);
DResult render_backend_destroy_component(struct RenderBackend *backend, RenderBackendComponent *component);
