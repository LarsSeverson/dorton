#include "render_backend_command_buffers.h"
#include "render_backend_command_buffer.h"
#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_command_buffers(RenderBackend *backend)
{
    u32 size = backend->swap_chain.images_count;
    if (size != darray_size(&backend->command_buffers.graphics_command_buffers))
    {
        if (darray_reserve(&backend->command_buffers.graphics_command_buffers, RenderBackendCommandBuffer, size) != D_SUCCESS)
        {
            return D_FATAL;
        }
    }

    for (u32 i = 0; i < size; ++i)
    {
        CommandBufferInfo command_buffer_info = {0};
        RenderBackendCommandBuffer *command_buffer = (RenderBackendCommandBuffer *)darray_get(&backend->command_buffers.graphics_command_buffers, i);
        if (command_buffer->command_buffer_inner != NULL)
        {
            render_backend_destroy_command_buffer(backend, command_buffer);
        }

        if (render_backend_create_command_buffer(backend, command_buffer, &command_buffer_info) != D_SUCCESS)
        {
            DFATAL("Could not create command buffers.");
            return D_FATAL;
        }
    }

    return D_SUCCESS;
}

DResult render_backend_destroy_command_buffers(RenderBackend *backend)
{
    for (u32 i = 0; i < darray_size(&backend->command_buffers.graphics_command_buffers); ++i)
    {
        RenderBackendCommandBuffer *command_buffer = (RenderBackendCommandBuffer *)darray_get(&backend->command_buffers.graphics_command_buffers, i);
        render_backend_destroy_command_buffer(backend, command_buffer);
    }

    darray_destroy(&backend->command_buffers.graphics_command_buffers);

    return D_SUCCESS;
}