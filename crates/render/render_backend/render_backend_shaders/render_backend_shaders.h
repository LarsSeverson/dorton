#pragma once

#include "render/render_types.h"
#include "darray/darray.h"

typedef struct RenderBackendShaders
{
    // RenderBackendShaders
    DArray shaders_inner;
} RenderBackendShaders;

struct RenderBackend;
struct RenderBackendShader;
struct ShaderInfo;

DResult render_backend_create_shaders(struct RenderBackend *backend);
DResult render_backend_destroy_shaders(struct RenderBackend *backend);

DResult render_backend_add_shader(struct RenderBackend *backend, struct RenderBackendShader *shader, struct ShaderInfo *shader_info);