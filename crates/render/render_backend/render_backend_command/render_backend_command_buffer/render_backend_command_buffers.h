#pragma once

#include "darray/darray.h"

#include "./render_backend_command_buffer_core.h"
#include "./render_backend_command_buffer.h"

typedef struct CommandBuffersInfo
{
  u32 size;
  CommandBufferInfo *command_buffer_info;

} CommandBuffersInfo;

typedef struct RenderBackendCommandBuffers
{
  // RenderBackendCommandBuffer
  DArray command_buffers;
  u32 size;

} RenderBackendCommandBuffers;

struct RenderBackend;

DResult render_backend_create_command_buffers(struct RenderBackend *backend, RenderBackendCommandBuffers *command_buffers, CommandBuffersInfo *command_buffers_info);
DResult render_backend_destroy_command_buffers(struct RenderBackend *backend, RenderBackendCommandBuffers *command_buffers);
