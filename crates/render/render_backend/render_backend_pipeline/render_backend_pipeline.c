#include "render_backend_pipeline.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

#include <string.h>

// TODO: Any create info values that are hard coded (e.g. input_assembly_info.primitiveRestardEnable = VK_FALSE), make configurable

VkPipelineVertexInputStateCreateInfo pipeline_create_vertex_input_info(PipelineInfo *pipeline_info)
{
    u32 binding_description_count = darray_size(&pipeline_info->binding_descriptions);
    u32 attribute_description_count = darray_size(&pipeline_info->attribute_descriptions);
    VkVertexInputBindingDescription *binding_descriptions = (VkVertexInputBindingDescription *)darray_data(&pipeline_info->binding_descriptions);
    VkVertexInputAttributeDescription *attribute_descriptions = (VkVertexInputAttributeDescription *)darray_data(&pipeline_info->attribute_descriptions);

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
    vertex_input_info.vertexBindingDescriptionCount = binding_description_count;
    vertex_input_info.pVertexBindingDescriptions = binding_descriptions;
    vertex_input_info.vertexAttributeDescriptionCount = attribute_description_count;
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions;

    return vertex_input_info;
}

VkPipelineShaderStageCreateInfo *pipeline_create_shader_stage_info(PipelineInfo *pipeline_info)
{
    return render_backend_shaders_get_stages(&pipeline_info->shaders);
}

VkPipelineInputAssemblyStateCreateInfo pipeline_create_input_assembly_info(PipelineInfo *pipeline_info)
{
    VkPipelineInputAssemblyStateCreateInfo input_assembly_info = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    input_assembly_info.topology = pipeline_info->topology;
    input_assembly_info.primitiveRestartEnable = VK_FALSE;

    return input_assembly_info;
}

VkPipelineViewportStateCreateInfo pipeline_create_viewport_info(PipelineInfo *pipeline_info)
{
    VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &pipeline_info->viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &pipeline_info->scissor;

    return viewport_state;
}

VkPipelineRasterizationStateCreateInfo pipeline_create_rasterization_info(PipelineInfo *pipeline_info)
{
    VkPipelineRasterizationStateCreateInfo rasterizer_info = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rasterizer_info.depthBiasEnable = VK_FALSE;
    rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_info.polygonMode = pipeline_info->rasterizer_info.polygon_mode;
    rasterizer_info.lineWidth = 1.0f;
    rasterizer_info.cullMode = pipeline_info->rasterizer_info.cull_mode;
    rasterizer_info.frontFace = pipeline_info->rasterizer_info.front_face;
    rasterizer_info.depthBiasEnable = VK_FALSE;

    return rasterizer_info;
}

VkPipelineMultisampleStateCreateInfo pipeline_create_multisample_info(PipelineInfo *pipeline_info)
{
    VkPipelineMultisampleStateCreateInfo multisample_info = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    multisample_info.flags = pipeline_info->multisample_info.flags;
    multisample_info.rasterizationSamples = pipeline_info->multisample_info.rasterization_samples;
    multisample_info.sampleShadingEnable = pipeline_info->multisample_info.sample_shading_enable;
    multisample_info.minSampleShading = pipeline_info->multisample_info.min_sample_shading;
    multisample_info.pSampleMask = pipeline_info->multisample_info.sample_mask;
    multisample_info.alphaToCoverageEnable = pipeline_info->multisample_info.alpha_to_coverage_enable;
    multisample_info.alphaToOneEnable = pipeline_info->multisample_info.alpha_to_one_enable;

    return multisample_info;
}

VkPipelineColorBlendStateCreateInfo pipeline_create_color_blend_info(PipelineInfo *pipeline_info)
{
    VkPipelineColorBlendStateCreateInfo color_blend_info = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    color_blend_info.flags = pipeline_info->color_blend_info.flags;
    color_blend_info.logicOpEnable = pipeline_info->color_blend_info.logic_operation_enable;
    color_blend_info.logicOp = pipeline_info->color_blend_info.logic_operation;
    color_blend_info.attachmentCount = (u32)darray_size(&pipeline_info->color_blend_info.attachments);
    color_blend_info.pAttachments = (VkPipelineColorBlendAttachmentState *)darray_data(&pipeline_info->color_blend_info.attachments);

    memcpy(color_blend_info.blendConstants, pipeline_info->color_blend_info.blend_constants, sizeof(color_blend_info.blendConstants));

    return color_blend_info;
}

DResult render_backend_create_pipeline(RenderBackend *backend, RenderBackendPipeline *pipeline, PipelineInfo *pipeline_info)
{
    VkPipelineShaderStageCreateInfo *shader_stages_info = pipeline_create_shader_stage_info(pipeline_info);

    VkPipelineVertexInputStateCreateInfo vertex_input_info = pipeline_create_vertex_input_info(pipeline_info);

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info = pipeline_create_input_assembly_info(pipeline_info);

    VkPipelineViewportStateCreateInfo viewport_info = pipeline_create_viewport_info(pipeline_info);

    VkPipelineRasterizationStateCreateInfo rasterizer_info = pipeline_create_rasterization_info(pipeline_info);

    VkPipelineMultisampleStateCreateInfo multisample_info = pipeline_create_multisample_info(pipeline_info);

    // TODO: Depth and stencil infos

    VkPipelineColorBlendStateCreateInfo color_blend_info = pipeline_create_color_blend_info(pipeline_info);

    return D_SUCCESS;
}