#pragma once

#include "render/render_backend/render_backend_core.h"

typedef struct RenderBackendCommandPool
{
    VkCommandPool command_pool_inner;
} RenderBackendCommandPool;

struct RenderBackend;

DResult render_backend_create_command_pool(struct RenderBackend *backend);
DResult render_backend_destroy_command_pool(struct RenderBackend *backend);
