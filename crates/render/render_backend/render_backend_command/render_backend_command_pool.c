#include "render_backend_command_pool.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"
#include "render/render_backend/render_backend_device/render_backend_physical_device.h"

DResult render_backend_create_command_pool(RenderBackend *backend)
{
    QueueFamilyIndices queue_family_indices = {0};
    find_queue_families(&queue_family_indices, backend->device.physical_device, backend->vulkan_context.surface);

    VkCommandPoolCreateInfo command_pool_create_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = queue_family_indices.graphics_family;

    if (vkCreateCommandPool(backend->device.logical_device, &command_pool_create_info, backend->vulkan_context.allocator, &backend->command_pool.command_pool_inner) != VK_SUCCESS)
    {
        DFATAL("Unabled to create command pool.");
        return D_FATAL;
    }

    return D_SUCCESS;
}

DResult render_backend_destroy_command_pool(RenderBackend *backend)
{
    vkDestroyCommandPool(backend->device.logical_device, backend->command_pool.command_pool_inner, backend->vulkan_context.allocator);
    return D_SUCCESS;
}
