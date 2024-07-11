#pragma once

#include "darray/darray.h"

#include "./render_backend_shader_core.h"
#include "./render_backend_shader.h"

typedef struct RenderBackendShaders
{
    // RenderBackendShader
    DArray shaders_inner;
    // VkPipelineShaderStageCreateInfo
    DArray shader_stages;
} RenderBackendShaders;

DResult render_backend_create_shaders(struct RenderBackend *backend, RenderBackendShaders *shaders);
DResult render_backend_destroy_shaders(struct RenderBackend *backend, RenderBackendShaders *shaders);

DResult render_backend_shaders_push(struct RenderBackend *backend, RenderBackendShaders *shaders, RenderBackendShader *shader, ShaderInfo *shader_info);

DResult render_backend_create_default_shaders(struct RenderBackend *backend, RenderBackendShaders *shaders);

DArray render_backend_shaders_get_stages(RenderBackendShaders *shaders);