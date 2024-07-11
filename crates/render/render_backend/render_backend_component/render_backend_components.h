#pragma once

#include "./render_backend_component.h"

typedef struct RenderBackendComponents
{
  // RenderBackendComponent
  DArray components;
  u32 components_size;

} RenderBackendComponents;

struct RenderBackend;

DResult render_backend_create_components(struct RenderBackend *backend, RenderBackendComponents *components);
DResult render_backend_destroy_components(struct RenderBackend *backend, RenderBackendComponents *components);

DResult render_backend_components_push(struct RenderBackend *backend, RenderBackendComponents *components, RenderBackendComponentInfo *component_info);
DResult render_backend_components_push_empty(struct RenderBackend *backend, RenderBackendComponents *components);

DResult render_backend_process_components(struct RenderBackend *backend, RenderBackendComponents *components, RenderBackendDrawPacket *draw_packet);

DResult render_backend_components_recreate_framebuffers(struct RenderBackend *backend, RenderBackendComponents *components);

// Support: Only 1 component, not multiple
typedef struct ComponentsPipelineInfo
{ // TODO: Multiple support
  u32 start;
  u32 count;

  // TODO: DArray of ComponentPipelineInfo
  ComponentPipelineInfo *component_pipeline_info;

} ComponentsPipelineInfo;

DResult render_backend_components_create_pipelines(struct RenderBackend *backend, RenderBackendComponents *components, ComponentsPipelineInfo *components_pipeline_info);
DResult render_backend_components_create_default_pipelines(struct RenderBackend *backend, RenderBackendComponents *components, u32 start, u32 count); 