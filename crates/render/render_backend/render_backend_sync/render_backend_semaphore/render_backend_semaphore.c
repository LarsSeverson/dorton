#include "render_backend_semaphore.h"

#include "logger.h"

DResult render_backend_create_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore)
{
    VkSemaphoreCreateInfo semaphore_create_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    if (vkCreateSemaphore(backend->device.logical_device, &semaphore_create_info, backend->vulkan_context.allocator, &semaphore->semaphore_inner) != VK_SUCCESS)
    {
        DERROR("Could not create semaphore.");
        return D_ERROR;
    }

    return D_SUCCESS;
}

DResult render_backend_destroy_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore)
{
    vkDestroySemaphore(backend->device.logical_device, semaphore->semaphore_inner, backend->vulkan_context.allocator);
    return D_SUCCESS;
}

dbool render_backend_wait_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore, u64 timeout_ns)
{
    return false;
}

DResult render_backend_reset_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore)
{
    return D_SUCCESS;
}