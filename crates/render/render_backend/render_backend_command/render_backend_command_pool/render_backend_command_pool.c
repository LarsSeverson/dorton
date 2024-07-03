#include "render_backend_command_pool.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"
#include "render/render_backend/render_backend_command/render_backend_command_buffer/render_backend_command_buffer.h"

DResult render_backend_create_command_pool(RenderBackend *backend, RenderBackendCommandPool *command_pool, CommandPoolInfo *command_pool_info)
{
    VkCommandPoolCreateInfo command_pool_create_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    command_pool_create_info.flags = command_pool_info->flags;
    command_pool_create_info.queueFamilyIndex = command_pool_info->queue_family_index;

    if (vkCreateCommandPool(backend->device.logical_device, &command_pool_create_info, backend->vulkan_context.allocator, &command_pool->command_pool_inner) != VK_SUCCESS)
    {
        DFATAL("Unabled to create command pool.");
        return D_FATAL;
    }

    command_pool->queue_family_index = command_pool_info->queue_family_index;

    return D_SUCCESS;
}

DResult render_backend_destroy_command_pool(RenderBackend *backend, RenderBackendCommandPool *command_pool)
{
    vkDestroyCommandPool(backend->device.logical_device, command_pool->command_pool_inner, backend->vulkan_context.allocator);

    *command_pool = (RenderBackendCommandPool){0};

    return D_SUCCESS;
}
