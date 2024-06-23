#pragma once

#include "render/render_backend/render_backend_index/render_backend_indices.h"
#include "render/render_backend/render_backend_buffer/render_backend_buffer.h"

typedef struct RenderBackendIndexBuffer
{   
    RenderBackendBuffer buffer;

    RenderBackendIndices indices;
} RenderBackendIndexBuffer;

struct RenderBackend;

DResult render_backend_create_index_buffer(struct RenderBackend *backend, RenderBackendIndexBuffer *index_buffer, RenderBackendIndices *indices);
DResult render_backend_destroy_index_buffer(struct RenderBackend *backend, RenderBackendIndexBuffer *index_buffer);
