#include "render_backend_index_buffer.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

#include <string.h>

DResult render_backend_create_index_buffer(RenderBackend *backend, RenderBackendIndexBuffer *index_buffer, RenderBackendIndices *indices)
{
    u32 indices_size = render_backend_indices_size(indices);
    u64 index_buffer_size = sizeof(RenderBackendIndex) * indices_size;

    BufferInfo staging_buffer_info = {VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
    staging_buffer_info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    staging_buffer_info.size = index_buffer_size;
    staging_buffer_info.offset = 0;

    RenderBackendBuffer staging_buffer;
    if (render_backend_create_buffer(backend, &staging_buffer, &staging_buffer_info) != D_SUCCESS)
    {
        DERROR("Could not create index buffer.");
        return D_ERROR;
    }

    RenderBackendIndex *indices_data = render_backend_indices_data(indices);

    void *data;
    vkMapMemory(backend->device.logical_device, staging_buffer.memory, 0, index_buffer_size, 0, &data);
    memcpy(data, indices_data, (size_t)index_buffer_size);
    vkUnmapMemory(backend->device.logical_device, staging_buffer.memory);

    BufferInfo index_buffer_info = {VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
    index_buffer_info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    index_buffer_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    index_buffer_info.size = index_buffer_size;
    index_buffer_info.offset = 0;

    if (render_backend_create_buffer(backend, &index_buffer->buffer, &index_buffer_info) != D_SUCCESS)
    {
        DERROR("Could not create index buffer.");
        return D_ERROR;
    }

    if (render_backend_copy_buffer(backend, &staging_buffer, &index_buffer->buffer, index_buffer_size) != D_SUCCESS)
    {
        DERROR("Could not create index buffer.");
        return D_ERROR;
    }

    render_backend_destroy_buffer(backend, &staging_buffer);

    return D_SUCCESS;
}

DResult render_backend_destroy_index_buffer(RenderBackend *backend, RenderBackendIndexBuffer *index_buffer)
{
    return D_SUCCESS;
}