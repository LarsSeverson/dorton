#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"
#include "render_backend_shader.h"

typedef struct RenderBackendShaders
{
    // RenderBackendShaders
    DArray shaders_inner;
} RenderBackendShaders;

struct RenderBackendShader;
struct ShaderInfo;

DResult render_backend_create_shaders(struct RenderBackend *backend, RenderBackendShaders *shaders);
DResult render_backend_destroy_shaders(struct RenderBackend *backend, RenderBackendShaders *shaders);

DResult render_backend_add_shader(struct RenderBackend *backend, RenderBackendShaders *shaders, struct RenderBackendShader *shader, struct ShaderInfo *shader_info);