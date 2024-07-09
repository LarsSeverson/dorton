#include "render_backend_command_buffer.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_command_buffer(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer, CommandBufferInfo *command_buffer_info)
{
    command_buffer->state = COMMAND_BUFFER_STATE_INVALID;

    VkCommandBufferAllocateInfo command_buffer_alloc_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    command_buffer_alloc_info.commandPool = command_buffer_info->command_pool->command_pool_inner;
    command_buffer_alloc_info.level = command_buffer_info->type == COMMAND_BUFFER_TYPE_PRIMARY ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    command_buffer_alloc_info.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(backend->device.logical_device, &command_buffer_alloc_info, &command_buffer->command_buffer_inner) != VK_SUCCESS)
    {
        DERROR("Could not create command buffer.");
        return D_ERROR;
    }

    command_buffer->state = COMMAND_BUFFER_STATE_INITIAL;
    command_buffer->use = command_buffer_info->use;
    command_buffer->type = command_buffer_info->type;

    return D_SUCCESS;
}

DResult render_backend_destroy_command_buffer(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer)
{
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
    vkResetCommandBuffer(command_buffer->command_buffer_inner, 0);
    
    command_buffer->state = COMMAND_BUFFER_STATE_INITIAL;

    return D_SUCCESS;
}

DResult command_buffer_begin_lazy(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer)
{
    CommandBufferInfo command_buffer_info = {COMMAND_BUFFER_TYPE_PRIMARY};
    command_buffer_info.use = COMMAND_BUFFER_USE_SINGLE;
    command_buffer_info.command_pool = &backend->transfer_command_pool;

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

DResult command_buffer_end_lazy(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer, VkQueue queue)
{
    if (command_buffer_end(command_buffer) != D_SUCCESS)
    {
        return D_ERROR;
    }

    // TODO: Remove this hard code
    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer->command_buffer_inner;

    if (vkQueueSubmit(queue, 1, &submit_info, NULL) != VK_SUCCESS)
    {
        return D_ERROR;
    }

    vkQueueWaitIdle(queue);

    render_backend_destroy_command_buffer(backend, command_buffer);

    return D_SUCCESS;
}

// TODO: Command buffer states - possibilities for this function: Invalid | Initial
DResult render_backend_record_command_buffer(RenderBackend *backend, CommandBufferRecordInfo *record_info)
{
    if (record_info->command_buffer->state != COMMAND_BUFFER_STATE_RECORDING)
    {
        DERROR("Could not record command buffer. Command buffer state was not COMMAND_BUFFER_STATE_RECORDING.");
        return D_ERROR;
    }

    VkCommandBuffer command_buffer = record_info->command_buffer->command_buffer_inner;

    VkRenderPassBeginInfo render_pass_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    render_pass_info.renderPass = record_info->pipeline->render_pass.render_pass_inner;
    render_pass_info.framebuffer = record_info->framebuffer->framebuffer_inner;
    render_pass_info.renderArea.offset = (VkOffset2D){0, 0}; // TODO: Configurable
    render_pass_info.renderArea.extent = backend->swap_chain.extent;
    VkClearValue clear_color = {0.f, 0.f, 0.f, 1.f}; // TODO: Configurable
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    // TODO: Pipeline bind point?
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, record_info->pipeline->pipeline_inner);

    vkCmdSetViewport(command_buffer, 0, 1, &record_info->pipeline->viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &record_info->pipeline->scissor);

    // TODO: Vertex buffers
    VkBuffer vertex_buffers[] = {record_info->vertex_buffer->buffer.buffer_inner};
    VkDeviceSize vertex_buffer_offsets[] = {record_info->vertex_buffer->buffer.offset};
    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, vertex_buffer_offsets);

    VkBuffer index_buffer = record_info->index_buffer->buffer.buffer_inner;
    VkDeviceSize index_buffer_offset = record_info->index_buffer->buffer.offset;
    // TODO: VkIndexType index_buffer_type = record_info->index_buffer.index_type;
    vkCmdBindIndexBuffer(command_buffer, index_buffer, index_buffer_offset, VK_INDEX_TYPE_UINT16);

    u32 index_count = render_backend_index_count(record_info->index_buffer);
    // TODO: Draw indexed function parameters configurable
    vkCmdDrawIndexed(command_buffer, index_count, 1, 0, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    return D_SUCCESS;
}