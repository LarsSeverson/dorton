#include "render_backend_command_buffers.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_command_buffers(RenderBackend *backend, RenderBackendCommandBuffers *command_buffers, CommandBuffersInfo *command_buffers_info)
{
  u32 size = command_buffers_info->size;

  darray_reserve(&command_buffers->command_buffers, RenderBackendCommandBuffer, size);

  for (u32 i = 0; i < size; ++i)
  {
    RenderBackendCommandBuffer *command_buffer = (RenderBackendCommandBuffer *)darray_get(&command_buffers->command_buffers, i);

    if (render_backend_create_command_buffer(backend, command_buffer, command_buffers_info->command_buffer_info) != D_SUCCESS)
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