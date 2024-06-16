#include "render_backend_shader.h"
#include "logger.h"

#include "../render_backend.h"

#include "utils/shader_utils_module.h"

DResult render_backend_create_shader(RenderBackend *backend, RenderBackendShader *shader, ShaderInfo *shader_info)
{
    DArray shader_code;
    DString file_path;

    const char *shader_type = shader_info->type == SHADER_TYPE_VERTEX ? "_vert" : "_frag";

    dstring_format(&file_path, "%s/%s%s.spv", SHADERS_DIR, shader_info->src, shader_type);

    if (read_shader_file(&shader_code, &file_path) != D_SUCCESS)
    {
        DERROR("Error reading shader file.");
        dstring_destroy(&file_path);

        return D_ERROR;
    }
    
    dstring_destroy(&file_path);

    if ((darray_size(&shader_code) % sizeof(u32) != 0))
    {
        DERROR("Shader code read is not castable to u32.");
        return D_ERROR;
    }


    VkShaderModuleCreateInfo shader_create_info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    shader_create_info.codeSize = (size_t)darray_size(&shader_code);
    shader_create_info.pCode = (u32 *)darray_data(&shader_code);

    if (vkCreateShaderModule(backend->device.logical_device, &shader_create_info, backend->vulkan_context.allocator, &shader->shader_inner) != VK_SUCCESS)
    {
        DERROR("Could not create shader module.");
        return D_ERROR;
    }

    shader->shader_stage = (VkPipelineShaderStageCreateInfo){VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};

    VkShaderStageFlagBits stage;
    switch (shader_info->type)
    {
    case SHADER_TYPE_VERTEX:
        stage = VK_SHADER_STAGE_VERTEX_BIT;
        break;
    case SHADER_TYPE_FRAGMENT:
        stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        break;
    }

    shader->shader_code = shader_code;
    shader->shader_stage.stage = stage;
    shader->shader_stage.module = shader->shader_inner;
    shader->shader_stage.pName = shader_info->main;

    shader->shader_info = *shader_info;

    return D_SUCCESS;
}

DResult render_backend_destroy_shader(RenderBackend *backend, RenderBackendShader *shader)
{
    vkDestroyShaderModule(backend->device.logical_device, shader->shader_inner, backend->vulkan_context.allocator);
    darray_destroy(&shader->shader_code);
    *shader = (RenderBackendShader){0};

    return D_SUCCESS;
}