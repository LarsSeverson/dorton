#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_command/render_backend_command_pool/render_backend_command_pool.h"

#include "./render_backend_command_buffer_core.h"
#include "./render_backend_command_buffer.h"

typedef struct CommandBuffersInfo
{
  u32 size;
  RenderBackendCommandPool *command_pool;

} CommandBuffersInfo;

typedef struct RenderBackendCommandBuffers
{
  // RenderBackendCommandBuffer
  DArray command_buffers;
  u32 size;

} RenderBackendCommandBuffers;

struct RenderBackend;

CommandBuffersInfo create_default_command_buffers_info(struct RenderBackend *backend);

DResult render_backend_create_command_buffers(struct RenderBackend *backend, RenderBackendCommandBuffers *command_buffers, CommandBuffersInfo *command_buffers_info);
DResult render_backend_destroy_command_buffers(struct RenderBackend *backend, RenderBackendCommandBuffers *command_buffers);

typedef struct CommandBuffersProcessInfo
{
  u32 start;
  u32 count;

  RenderBackendPipeline *pipeline;
  RenderBackendFramebuffer *framebuffer;
  RenderBackendVertexBuffer *vertex_buffer;
  RenderBackendIndexBuffer *index_buffer;

  RenderBackendCommandBuffer *command_buffer;

} CommandBuffersProcessInfo;

DResult render_backend_process_command_buffers(struct RenderBackend *backend, RenderBackendCommandBuffers *command_buffers, CommandBuffersProcessInfo *process_info);
