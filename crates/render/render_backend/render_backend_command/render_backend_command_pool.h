#pragma once

#include "render/render_types.h"

// ../render_backend.h
struct RenderBackend;

typedef struct RenderBackendCommandPool
{
    VkCommandPool command_pool_inner;
} RenderBackendCommandPool;

DResult render_backend_create_command_pool(struct RenderBackend *backend);
DResult render_backend_destroy_command_pool(struct RenderBackend *backend);
