#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_command/render_backend_command_core.h"

typedef struct CommandPoolInfo
{
    VkCommandPoolCreateFlags flags;
    u32 queue_family_index;

} CommandPoolInfo;

typedef struct RenderBackendCommandPool
{
    VkCommandPool command_pool_inner;
    u32 queue_family_index;

} RenderBackendCommandPool;

struct RenderBackend;

DResult render_backend_create_command_pool(struct RenderBackend *backend, RenderBackendCommandPool *command_pool, CommandPoolInfo *command_pool_info);
DResult render_backend_destroy_command_pool(struct RenderBackend *backend, RenderBackendCommandPool *command_pool);
