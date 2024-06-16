#pragma once

#include "render/render_types.h"
#include "darray/darray.h"

typedef enum ShaderType
{
    SHADER_TYPE_VERTEX = 0,
    SHADER_TYPE_FRAGMENT
} ShaderType;

typedef struct ShaderInfo
{
    ShaderType type;
    const char *src;
    const char *main;
} ShaderInfo;

typedef struct RenderBackendShader
{
    VkShaderModule shader_inner;

    // char
    DArray shader_code;
    ShaderInfo shader_info;
    VkPipelineShaderStageCreateInfo shader_stage;
} RenderBackendShader;

struct RenderBackend;

DResult render_backend_create_shader(struct RenderBackend *backend, RenderBackendShader *shader, ShaderInfo *shader_info);
DResult render_backend_destroy_shader(struct RenderBackend *backend, RenderBackendShader *shader);