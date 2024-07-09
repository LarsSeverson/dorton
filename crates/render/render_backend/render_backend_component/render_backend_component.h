#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"
#include "render/render_backend/render_backend_render_pass/render_backend_render_pass.h"
#include "render/render_backend/render_backend_framebuffer/render_backend_framebuffers.h"
#include "render/render_backend/render_backend_command/render_backend_command_pool/render_backend_command_pool.h"
#include "render/render_backend/render_backend_command/render_backend_command_buffer/render_backend_command_buffers.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex_buffer/render_backend_vertex_buffer.h"
#include "render/render_backend/render_backend_index/render_backend_index_buffer/render_backend_index_buffer.h"
#include "render/render_backend/render_backend_pipeline/render_backend_pipeline.h"

typedef struct RenderBackendComponentInfo
{
  RenderBackendRenderPass *render_pass;

  RenderBackendCommandPool *command_pool;
  RenderBackendCommandBuffers *command_buffers;

  RenderBackendVertexBuffer *vertex_buffer;
  RenderBackendIndexBuffer *index_buffer;

} RenderBackendComponentInfo;

typedef struct RenderBackendComponent
{
  RenderBackendRenderPass render_pass;
  RenderBackendFramebuffers framebuffers;

  RenderBackendCommandPool command_pool;
  RenderBackendCommandBuffers command_buffers;

  RenderBackendVertexBuffer vertex_buffer;
  RenderBackendIndexBuffer index_buffer;

  RenderBackendPipeline pipeline;

  dbool resize;

} RenderBackendComponent;

struct RenderBackend;

DResult render_backend_create_component(struct RenderBackend *backend, RenderBackendComponent *component, RenderBackendComponentInfo *component_info);
DResult render_backend_destroy_component(struct RenderBackend *backend, RenderBackendComponent *component);

DResult render_backend_component_set_vertices(RenderBackendComponent *component, RenderBackendVertices *vertices);
DResult render_backend_component_set_indices(RenderBackendComponent *component, RenderBackendIndices *indices);
DResult render_backend_component_set_pipeline(RenderBackendComponent *component, RenderBackendPipeline *pipeline);

DResult render_backend_process_component(struct RenderBackend *backend, RenderBackendComponent *component, RenderBackendDrawPacket *draw_packet);

DResult render_backend_recreate_component_framebuffers(struct RenderBackend *backend, RenderBackendComponent *component);