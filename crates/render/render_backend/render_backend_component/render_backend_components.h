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