#pragma once

#include "render/render_types.h"
#include "render/render_backend/render_backend.h"

typedef enum CommandBufferState
{
    COMMAND_BUFFER_STATE_INVALID = 0,
    COMMAND_BUFFER_STATE_INITIAL,
    COMMAND_BUFFER_STATE_RECORDING,
    COMMAND_BUFFER_STATE_EXCECUTABLE,
    COMMAND_BUFFER_STATE_PENDING,
} CommandBufferState;

typedef enum CommandBufferType
{
    COMMAND_BUFFER_TYPE_PRIMARY = 0,
    COMMAND_BUFFER_TYPE_SECONDARY
} CommandBufferType;

typedef enum CommandBufferUse
{
    COMMAND_BUFFER_USE_SINGLE = 0,
    COMMAND_BUFFER_USE_CONTINUE,
    COMMAND_BUFFER_USE_SIMULTANEOUS
} CommandBufferUse;

typedef struct CommandBufferInfo
{
    CommandBufferState state;
    CommandBufferType type;
    CommandBufferUse use;

    VkQueue queue;

} CommandBufferInfo;

typedef struct RenderBackendCommandBuffer
{
    VkCommandBuffer command_buffer_inner;

    CommandBufferState state;
    CommandBufferType type;
    CommandBufferUse use;

    VkQueue queue;
} RenderBackendCommandBuffer;


DResult render_backend_create_command_buffer(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer, CommandBufferInfo *command_buffer_info);
DResult render_backend_destroy_command_buffer(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer);

DResult command_buffer_begin(RenderBackendCommandBuffer *command_buffer);
DResult command_buffer_end(RenderBackendCommandBuffer *command_buffer);

DResult command_buffer_submit(RenderBackendCommandBuffer *command_buffer);
DResult command_buffer_reset(RenderBackendCommandBuffer *command_buffer);

DResult command_buffer_begin_lazy(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer);
DResult command_buffer_end_lazy(RenderBackend *backend, RenderBackendCommandBuffer *command_buffer);