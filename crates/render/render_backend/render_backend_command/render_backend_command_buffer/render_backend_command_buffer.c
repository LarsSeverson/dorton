#include "render_backend_command_buffer.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_command_buffer(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer, CommandBufferInfo *command_buffer_info)
{
    command_buffer->state = COMMAND_BUFFER_STATE_INVALID;

    VkCommandBufferAllocateInfo command_buffer_alloc_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    command_buffer_alloc_info.commandPool = backend->command_pool.command_pool_inner;
    command_buffer_alloc_info.level = command_buffer_info->type == COMMAND_BUFFER_TYPE_PRIMARY ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    command_buffer_alloc_info.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(backend->device.logical_device, &command_buffer_alloc_info, &command_buffer->command_buffer_inner) != VK_SUCCESS)
    {
        DFATAL("Could not create command buffer.");
        return D_FATAL;
    }

    command_buffer->state = COMMAND_BUFFER_STATE_INITIAL;
    command_buffer->use = command_buffer_info->use;
    command_buffer->type = command_buffer_info->type;

    return D_SUCCESS;
}

DResult render_backend_destroy_command_buffer(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer)
{
    vkFreeCommandBuffers(backend->device.logical_device, backend->command_pool.command_pool_inner, 1, &command_buffer->command_buffer_inner);
    command_buffer->command_buffer_inner = NULL;
    command_buffer->state = COMMAND_BUFFER_STATE_INVALID;

    return D_SUCCESS;
}

DResult command_buffer_begin(RenderBackendCommandBuffer *command_buffer)
{
    VkCommandBufferBeginInfo command_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

    if (command_buffer->use == COMMAND_BUFFER_USE_SINGLE)
    {
        command_buffer_begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if (command_buffer->use == COMMAND_BUFFER_USE_CONTINUE)
    {
        command_buffer_begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if (command_buffer->use == VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT)
    {
        command_buffer_begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    if (vkBeginCommandBuffer(command_buffer->command_buffer_inner, &command_buffer_begin_info) != VK_SUCCESS)
    {
        DERROR("Could not begin command buffer.");
        return D_ERROR;
    }

    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING;

    return D_SUCCESS;
}

DResult command_buffer_end(RenderBackendCommandBuffer *command_buffer)
{
    if (vkEndCommandBuffer(command_buffer->command_buffer_inner) != VK_SUCCESS)
    {
        command_buffer->state = COMMAND_BUFFER_STATE_INVALID;

        DERROR("Could not end command buffer.");
        return D_ERROR;
    }

    command_buffer->state = COMMAND_BUFFER_STATE_EXCECUTABLE;

    return D_SUCCESS;
}

DResult command_buffer_submit(RenderBackendCommandBuffer *command_buffer)
{
    // TODO: Submit the buffer
    command_buffer->state = COMMAND_BUFFER_STATE_PENDING;
    return D_SUCCESS;
}

DResult command_buffer_reset(RenderBackendCommandBuffer *command_buffer)
{
    // TODO: Reset the buffer
    command_buffer->state = COMMAND_BUFFER_STATE_INVALID;
    return D_SUCCESS;
}

DResult command_buffer_begin_lazy(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer)
{
    CommandBufferInfo command_buffer_info = {};
    command_buffer_info.use = COMMAND_BUFFER_USE_SINGLE;
    command_buffer_info.type = COMMAND_BUFFER_TYPE_PRIMARY;

    if (render_backend_create_command_buffer(backend, command_buffer, &command_buffer_info) != D_SUCCESS)
    {
        return D_ERROR;
    }

    if (command_buffer_begin(command_buffer) != D_SUCCESS)
    {
        return D_ERROR;
    }

    return D_SUCCESS;
}

DResult command_buffer_end_lazy(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer)
{
    if (command_buffer_end(command_buffer) != D_SUCCESS)
    {
        return D_ERROR;
    }

    // TODO: Remove this hard code
    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer->command_buffer_inner;

    if (vkQueueSubmit(command_buffer->queue, 1, &submit_info, NULL) != VK_SUCCESS)
    {   
        return D_ERROR;
    }

    vkQueueWaitIdle(command_buffer->queue);

    render_backend_destroy_command_buffer(backend, command_buffer);

    return D_SUCCESS;
}