#include "render_backend_shaders.h"
#include "render_backend_shader.h"

#include "logger.h"

#include "../render_backend.h"

DResult render_backend_create_shaders(RenderBackend *backend)
{
    darray_create(&backend->shaders.shaders_inner, RenderBackendShader);

    ShaderInfo vert_shader_info = {SHADER_TYPE_VERTEX};
    vert_shader_info.main = "main";
    vert_shader_info.src = "shader";

    RenderBackendShader vert_shader = {0};
    if (render_backend_add_shader(backend, &vert_shader, &vert_shader_info) != D_SUCCESS)
    {
        DFATAL("Could not create backend shaders.");
        return D_FATAL;
    }

    ShaderInfo frag_shader_info = {SHADER_TYPE_FRAGMENT};
    frag_shader_info.main = "main";
    frag_shader_info.src = "shader";

    RenderBackendShader frag_shader = {0};
    if (render_backend_add_shader(backend, &frag_shader, &frag_shader_info) != D_SUCCESS)
    {
        DFATAL("Could not create backend shaders.");
        return D_FATAL;
    }

    return D_SUCCESS;
}

DResult render_backend_destroy_shaders(RenderBackend *backend)
{
    for (u32 i = 0; i < darray_size(&backend->shaders.shaders_inner); ++i)
    {
        RenderBackendShader *shader = (RenderBackendShader *)darray_get(&backend->shaders.shaders_inner, i);
        render_backend_destroy_shader(backend, shader);
    }

    darray_destroy(&backend->shaders.shaders_inner);

    return D_SUCCESS;
}

DResult render_backend_add_shader(RenderBackend *backend, RenderBackendShader *shader, ShaderInfo *shader_info)
{
    if (render_backend_create_shader(backend, shader, shader_info) != VK_SUCCESS)
    {
        DERROR("Could not add shader.");
        return D_ERROR;
    }

    darray_push(&backend->shaders.shaders_inner, *shader);

    return D_SUCCESS;
}