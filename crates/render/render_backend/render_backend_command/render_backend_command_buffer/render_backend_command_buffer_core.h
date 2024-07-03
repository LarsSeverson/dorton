#pragma once

#include "render/render_backend/render_backend_command/render_backend_command_core.h"

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



