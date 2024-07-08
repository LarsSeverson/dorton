#include "render_backend_index_buffer.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

#include <string.h>

#define DEFAULT_INDEX_BUFFER_SIZE sizeof(RenderBackendIndex) * 4096

DResult render_backend_create_empty_index_buffer(RenderBackend *backend, RenderBackendIndexBuffer *index_buffer)
{
    u64 index_buffer_size = DEFAULT_INDEX_BUFFER_SIZE;

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

    void *data;
    vkMapMemory(backend->device.logical_device, staging_buffer.memory, 0, index_buffer_size, 0, &data);
    memset(data, 0, (size_t)index_buffer_size);
    vkUnmapMemory(backend->device.logical_device, staging_buffer.memory);

    BufferInfo index_buffer_create_info = {VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
    index_buffer_create_info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    index_buffer_create_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    index_buffer_create_info.size = index_buffer_size;
    index_buffer_create_info.offset = 0;

    if (render_backend_create_buffer(backend, &index_buffer->buffer, &index_buffer_create_info) != D_SUCCESS)
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

    index_buffer->indices = (RenderBackendIndices){0};

    return D_SUCCESS;
}

DResult render_backend_create_index_buffer(RenderBackend *backend, RenderBackendIndexBuffer *index_buffer, IndexBufferInfo *index_buffer_info)
{
    u32 indices_size = render_backend_indices_size(&index_buffer_info->indices);
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

    RenderBackendIndex *indices_data = render_backend_indices_data(&index_buffer_info->indices);

    void *data;
    vkMapMemory(backend->device.logical_device, staging_buffer.memory, 0, index_buffer_size, 0, &data);
    memcpy(data, indices_data, (size_t)index_buffer_size);
    vkUnmapMemory(backend->device.logical_device, staging_buffer.memory);

    BufferInfo index_buffer_create_info = {VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
    index_buffer_create_info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    index_buffer_create_info.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    index_buffer_create_info.size = index_buffer_size;
    index_buffer_create_info.offset = 0;

    if (render_backend_create_buffer(backend, &index_buffer->buffer, &index_buffer_create_info) != D_SUCCESS)
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

    index_buffer->indices = index_buffer_info->indices;

    return D_SUCCESS;
}

DResult render_backend_destroy_index_buffer(RenderBackend *backend, RenderBackendIndexBuffer *index_buffer)
{
    render_backend_destroy_buffer(backend, &index_buffer->buffer);

    darray_destroy(&index_buffer->indices);

    *index_buffer = (RenderBackendIndexBuffer){0};

    return D_SUCCESS;
}

u32 render_backend_index_count(RenderBackendIndexBuffer *index_buffer)
{
    return (u32)darray_size(&index_buffer->indices);
}