#pragma once

#include "render/render_backend/render_backend_core.h"

typedef u32 ShaderFlagBits;
typedef enum ShaderFlags
{
    SHADER_FLAG_NONE = 0x0,
    SHADER_FLAG_OUTLINE = 0x1
} ShaderFlags;

typedef enum ShaderType
{
    SHADER_TYPE_VERTEX = 0,
    SHADER_TYPE_FRAGMENT
} ShaderType;