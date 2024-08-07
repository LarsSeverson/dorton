#include "render_backend_shaders.h"
#include "render_backend_shader.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_shaders(RenderBackend *backend, RenderBackendShaders *shaders)
{
    darray_create(&shaders->shaders_inner, RenderBackendShader);
    darray_create(&shaders->shader_stages, VkPipelineShaderStageCreateInfo);

    return D_SUCCESS;
}

DResult render_backend_destroy_shaders(RenderBackend *backend, RenderBackendShaders *shaders)
{
    for (u32 i = 0; i < darray_size(&shaders->shaders_inner); ++i)
    {
        RenderBackendShader *shader = (RenderBackendShader *)darray_get(&shaders->shaders_inner, i);
        render_backend_destroy_shader(backend, shader);
    }

    darray_destroy(&shaders->shaders_inner);
    darray_destroy(&shaders->shader_stages);

    return D_SUCCESS;
}

DResult render_backend_shaders_push(RenderBackend *backend, RenderBackendShaders *shaders, RenderBackendShader *shader, ShaderInfo *shader_info)
{
    if (render_backend_create_shader(backend, shader, shader_info) != VK_SUCCESS)
    {
        DERROR("Could not add shader.");
        return D_ERROR;
    }

    darray_push(&shaders->shaders_inner, *shader);
    darray_push(&shaders->shader_stages, shader->shader_stage);

    return D_SUCCESS;
}

DResult render_backend_create_default_shaders(RenderBackend *backend, RenderBackendShaders *shaders)
{
    render_backend_create_shaders(backend, shaders);

    ShaderInfo vert_shader_info = {SHADER_TYPE_VERTEX};
    vert_shader_info.main = "main";
    vert_shader_info.src = "default";

    RenderBackendShader vert_shader = {0};
    if (render_backend_shaders_push(backend, shaders, &vert_shader, &vert_shader_info) != D_SUCCESS)
    {
        DFATAL("Could not create backend shaders.");
        return D_FATAL;
    }

    ShaderInfo frag_shader_info = {SHADER_TYPE_FRAGMENT};
    frag_shader_info.main = "main";
    frag_shader_info.src = "default";

    RenderBackendShader frag_shader = {0};
    if (render_backend_shaders_push(backend, shaders, &frag_shader, &frag_shader_info) != D_SUCCESS)
    {
        DFATAL("Could not create backend shaders.");
        return D_FATAL;
    }

    return D_SUCCESS;
}

DArray render_backend_shaders_get_stages(RenderBackendShaders *shaders)
{
    return shaders->shader_stages;
}
