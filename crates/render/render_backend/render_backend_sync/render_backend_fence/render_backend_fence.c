#include "render_backend_fence.h"

#include "logger.h"

DResult render_backend_create_fence(RenderBackend *backend, RenderBackendFence *fence)
{
    VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(backend->device.logical_device, &fence_create_info, backend->vulkan_context.allocator, &fence->fence_inner) != VK_SUCCESS)
    {
        DERROR("Could not create fence.");
        return D_ERROR;
    }

    fence->fence_state = FENCE_STATE_SIGNALED;

    return D_SUCCESS;
}

DResult render_backend_destroy_fence(RenderBackend *backend, RenderBackendFence *fence)
{
    vkDestroyFence(backend->device.logical_device, fence->fence_inner, backend->vulkan_context.allocator);

    fence->fence_state = FENCE_STATE_NOT_SIGNALED;

    return D_SUCCESS;
}

dbool render_backend_wait_for_fence(RenderBackend *backend, RenderBackendFence *fence, u64 timeout_ns)
{
    if (fence->fence_state == FENCE_STATE_SIGNALED)
    {
        return true;
    }

    VkResult wait_result = vkWaitForFences(backend->device.logical_device, 1, &fence->fence_inner, VK_TRUE, timeout_ns);

    switch (wait_result)
    {
    case VK_SUCCESS:
        fence->fence_state = FENCE_STATE_SIGNALED;
        return true;
    case VK_TIMEOUT:
        DWARN("Fence timed out.");
        break;
    case VK_ERROR_DEVICE_LOST:
        DERROR("Fence device lost.");
        break;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        DERROR("Fence out of host memory.");
        break;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        DERROR("Fence out of device memory.");
        break;
    default:
        DERROR("Fence unknown error occured.");
        break;
    }

    return false;
}

DResult render_backend_reset_fence(RenderBackend *backend, RenderBackendFence *fence)
{
    if (fence->fence_state == FENCE_STATE_SIGNALED)
    {
        if (vkResetFences(backend->device.logical_device, 1, &fence->fence_inner) != VK_SUCCESS)
        {
            DERROR("An error occured trying to reset a fence.");
            return D_ERROR;
        }

        fence->fence_state = FENCE_STATE_NOT_SIGNALED;

        return D_SUCCESS;
    }

    return D_IGNORED;
}
