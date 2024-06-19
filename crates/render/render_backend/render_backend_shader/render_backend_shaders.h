#pragma once

#include "render/render_types.h"
#include "darray/darray.h"

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