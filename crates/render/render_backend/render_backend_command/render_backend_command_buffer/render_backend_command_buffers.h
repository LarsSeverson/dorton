#pragma once

#include "darray/darray.h"

typedef struct RenderBackendCommandBuffers
{
    // RenderBackendCommandBuffer
    DArray graphics_command_buffers;
} RenderBackendCommandBuffers;

struct RenderBackend;

DResult render_backend_create_command_buffers(struct RenderBackend *backend);
DResult render_backend_destroy_command_buffers(struct RenderBackend *backend);