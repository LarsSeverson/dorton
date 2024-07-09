#include "render_backend_command_buffers.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

CommandBuffersInfo create_default_command_buffers_info(RenderBackend *backend)
{
  CommandBuffersInfo command_buffers_info = {0};
  command_buffers_info.size = backend->swap_chain.max_frames_in_flight;

  return command_buffers_info;
}

DResult render_backend_create_command_buffers(RenderBackend *backend, RenderBackendCommandBuffers *command_buffers, CommandBuffersInfo *command_buffers_info)
{
  u32 size = command_buffers_info->size;

  darray_reserve(&command_buffers->command_buffers, RenderBackendCommandBuffer, size);

  CommandBufferInfo command_buffer_info = {COMMAND_BUFFER_TYPE_PRIMARY};
  command_buffer_info.command_pool = command_buffers_info->command_pool;

  for (u32 i = 0; i < size; ++i)
  {
    RenderBackendCommandBuffer *command_buffer = (RenderBackendCommandBuffer *)darray_get(&command_buffers->command_buffers, i);

    if (render_backend_create_command_buffer(backend, command_buffer, &command_buffer_info) != D_SUCCESS)
    {
      DERROR("Could not create command buffers.");
      return D_ERROR;
    }
  }

  command_buffers->size = size;

  return D_SUCCESS;
}

DResult render_backend_destroy_command_buffers(RenderBackend *backend, RenderBackendCommandBuffers *command_buffers)
{
  for (u32 i = 0; i < command_buffers->size; ++i)
  {
    RenderBackendCommandBuffer *command_buffer = (RenderBackendCommandBuffer *)darray_get(&command_buffers->command_buffers, i);

    render_backend_destroy_command_buffer(backend, command_buffer);
  }

  darray_destroy(&command_buffers->command_buffers);

  *command_buffers = (RenderBackendCommandBuffers){0};

  return D_SUCCESS;
}

// TODO: Start and count for multiple processing of command buffers. For now start is simply the index to process
DResult render_backend_process_command_buffers(RenderBackend *backend, RenderBackendCommandBuffers *command_buffers, CommandBuffersProcessInfo *process_info)
{
  RenderBackendCommandBuffer *command_buffer = (RenderBackendCommandBuffer *)darray_get(&command_buffers->command_buffers, process_info->start);

  command_buffer_reset(command_buffer);

  command_buffer_begin(command_buffer);

  CommandBufferRecordInfo record_info = {0};
  record_info.command_buffer = command_buffer;
  record_info.pipeline = process_info->pipeline;
  record_info.framebuffer = process_info->framebuffer;
  record_info.vertex_buffer = process_info->vertex_buffer;
  record_info.index_buffer = process_info->index_buffer;

  if (render_backend_record_command_buffer(backend, &record_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  command_buffer_end(command_buffer);

  process_info->command_buffer = command_buffer;

  return D_SUCCESS;
}