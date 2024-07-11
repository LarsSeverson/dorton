#pragma once

#include "render/render_backend/render_backend_index/render_backend_indices.h"
#include "render/render_backend/render_backend_buffer/render_backend_buffer.h"

typedef struct IndexBufferInfo
{
    RenderBackendIndices *indices;

} IndexBufferInfo;

typedef struct RenderBackendIndexBuffer
{
    RenderBackendBuffer buffer;
    RenderBackendIndices indices;

} RenderBackendIndexBuffer;

struct RenderBackend;

DResult render_backend_create_empty_index_buffer(struct RenderBackend *backend, RenderBackendIndexBuffer *index_buffer);
DResult render_backend_create_index_buffer(struct RenderBackend *backend, RenderBackendIndexBuffer *index_buffer, IndexBufferInfo *index_buffer_info);
DResult render_backend_destroy_index_buffer(struct RenderBackend *backend, RenderBackendIndexBuffer *index_buffer);

DResult render_backend_update_index_buffer(struct RenderBackend *backend, RenderBackendIndexBuffer *index_buffer, RenderBackendIndices *indices);

u32 render_backend_index_count(RenderBackendIndexBuffer *index_buffer);