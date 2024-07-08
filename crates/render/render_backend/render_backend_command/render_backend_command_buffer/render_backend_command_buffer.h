#pragma once

#include "./render_backend_command_buffer_core.h"

#include "render/render_backend/render_backend_command/render_backend_command_pool/render_backend_command_pool.h"
#include "render/render_backend/render_backend_pipeline/render_backend_pipeline.h"
#include "render/render_backend/render_backend_vertex/render_backend_vertex_lib.h"
#include "render/render_backend/render_backend_index/render_backend_index_lib.h"
#include "render/render_backend/render_backend_framebuffer/render_backend_framebuffer.h"

typedef struct CommandBufferInfo
{
    CommandBufferType type;
    CommandBufferState state;
    CommandBufferUse use;

    RenderBackendCommandPool *command_pool;

} CommandBufferInfo;

typedef struct RenderBackendCommandBuffer
{
    VkCommandBuffer command_buffer_inner;

    CommandBufferState state;
    CommandBufferType type;
    CommandBufferUse use;

} RenderBackendCommandBuffer;

struct RenderBackend;

DResult render_backend_create_command_buffer(struct RenderBackend *backend, RenderBackendCommandBuffer *command_buffer, CommandBufferInfo *command_buffer_info);
DResult render_backend_destroy_command_buffer(struct RenderBackend *backend, RenderBackendCommandBuffer *command_buffer);

DResult command_buffer_begin(RenderBackendCommandBuffer *command_buffer);
DResult command_buffer_end(RenderBackendCommandBuffer *command_buffer);

DResult command_buffer_submit(RenderBackendCommandBuffer *command_buffer);
DResult command_buffer_reset(RenderBackendCommandBuffer *command_buffer);

DResult command_buffer_begin_lazy(struct RenderBackend *backend, RenderBackendCommandBuffer *command_buffer);
DResult command_buffer_end_lazy(struct RenderBackend *backend, RenderBackendCommandBuffer *command_buffer, VkQueue queue);

typedef struct CommandBufferRecordInfo
{
    RenderBackendCommandBuffer *command_buffer;

    RenderBackendPipeline *pipeline;
    RenderBackendFramebuffer *framebuffer;
    RenderBackendVertexBuffer *vertex_buffer;
    RenderBackendIndexBuffer *index_buffer;
    // TODO: Descriptor set

} CommandBufferRecordInfo;

DResult render_backend_record_command_buffer(struct RenderBackend *backend, CommandBufferRecordInfo *record_info);