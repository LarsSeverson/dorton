#include "render_backend_pipeline.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

#include <string.h>

// TODO: Any create info values that are hard coded (e.g. input_assembly_info.primitiveRestardEnable = VK_FALSE), make configurable

VkPipelineShaderStageCreateInfo *pipeline_create_shader_stage_info(PipelineInfo *pipeline_info)
{
    DArray shader_stages = render_backend_shaders_get_stages(pipeline_info->shaders);

    VkPipelineShaderStageCreateInfo *shader_stages_data = (VkPipelineShaderStageCreateInfo *)darray_data(&shader_stages);

    return shader_stages_data;
}

VkPipelineVertexInputStateCreateInfo pipeline_create_vertex_input_info(PipelineInfo *pipeline_info)
{
    u32 binding_description_count = darray_size(pipeline_info->binding_descriptions);
    u32 attribute_description_count = darray_size(pipeline_info->attribute_descriptions);
    VkVertexInputBindingDescription *binding_descriptions = (VkVertexInputBindingDescription *)darray_data(pipeline_info->binding_descriptions);
    VkVertexInputAttributeDescription *attribute_descriptions = (VkVertexInputAttributeDescription *)darray_data(pipeline_info->attribute_descriptions);

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
    vertex_input_info.vertexBindingDescriptionCount = binding_description_count;
    vertex_input_info.pVertexBindingDescriptions = binding_descriptions;
    vertex_input_info.vertexAttributeDescriptionCount = attribute_description_count;
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions;

    return vertex_input_info;
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
    viewport_state.viewportCount = pipeline_info->viewport_count;
    viewport_state.scissorCount = pipeline_info->scissor_count;

    return viewport_state;
}

VkPipelineRasterizationStateCreateInfo pipeline_create_rasterization_info(PipelineInfo *pipeline_info)
{
    VkPipelineRasterizationStateCreateInfo rasterizer_info = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rasterizer_info.depthBiasEnable = VK_FALSE;
    rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_info.polygonMode = pipeline_info->rasterizer_info->polygon_mode;
    rasterizer_info.lineWidth = 1.0f;
    rasterizer_info.cullMode = pipeline_info->rasterizer_info->cull_mode;
    rasterizer_info.frontFace = pipeline_info->rasterizer_info->front_face;
    rasterizer_info.depthBiasEnable = VK_FALSE;

    return rasterizer_info;
}

VkPipelineMultisampleStateCreateInfo pipeline_create_multisample_info(PipelineInfo *pipeline_info)
{
    VkPipelineMultisampleStateCreateInfo multisample_info = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    multisample_info.flags = pipeline_info->multisample_info->flags;
    multisample_info.rasterizationSamples = pipeline_info->multisample_info->rasterization_samples;
    multisample_info.sampleShadingEnable = pipeline_info->multisample_info->sample_shading_enable;
    multisample_info.minSampleShading = pipeline_info->multisample_info->min_sample_shading;
    multisample_info.pSampleMask = pipeline_info->multisample_info->sample_mask;
    multisample_info.alphaToCoverageEnable = pipeline_info->multisample_info->alpha_to_coverage_enable;
    multisample_info.alphaToOneEnable = pipeline_info->multisample_info->alpha_to_one_enable;

    return multisample_info;
}

VkPipelineColorBlendStateCreateInfo pipeline_create_color_blend_info(PipelineInfo *pipeline_info)
{
    VkPipelineColorBlendStateCreateInfo color_blend_info = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    color_blend_info.flags = pipeline_info->color_blend_info->flags;
    color_blend_info.logicOpEnable = pipeline_info->color_blend_info->logic_operation_enable;
    color_blend_info.logicOp = pipeline_info->color_blend_info->logic_operation;
    color_blend_info.attachmentCount = (u32)darray_size(&pipeline_info->color_blend_info->attachments);
    color_blend_info.pAttachments = (VkPipelineColorBlendAttachmentState *)darray_data(&pipeline_info->color_blend_info->attachments);

    memcpy(color_blend_info.blendConstants, pipeline_info->color_blend_info->blend_constants, sizeof(color_blend_info.blendConstants));

    return color_blend_info;
}

VkPipelineDynamicStateCreateInfo pipeline_create_dynamic_state_info(PipelineInfo *pipeline_info)
{
    VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
    dynamic_state.dynamicStateCount = (u32)darray_size(pipeline_info->dynamic_states);
    dynamic_state.pDynamicStates = (VkDynamicState *)darray_data(pipeline_info->dynamic_states);

    return dynamic_state;
}

VkPipelineLayoutCreateInfo pipeline_create_layout_info(PipelineInfo *pipeline_info)
{
    VkPipelineLayoutCreateInfo pipeline_layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pipeline_layout_info.flags = pipeline_info->pipeline_layout->flags;
    pipeline_layout_info.setLayoutCount = (u32)darray_size(&pipeline_info->pipeline_layout->descriptor_set_layouts);
    pipeline_layout_info.pSetLayouts = (VkDescriptorSetLayout *)darray_data(&pipeline_info->pipeline_layout->descriptor_set_layouts);
    pipeline_layout_info.pushConstantRangeCount = (u32)darray_size(&pipeline_info->pipeline_layout->push_constant_ranges);
    pipeline_layout_info.pPushConstantRanges = (VkPushConstantRange *)darray_data(&pipeline_info->pipeline_layout->push_constant_ranges);

    return pipeline_layout_info;
}

DResult render_backend_destroy_pipeline_info(PipelineInfo *pipeline_info);

DResult render_backend_create_pipeline(RenderBackend *backend, RenderBackendPipeline *pipeline, PipelineInfo *pipeline_info)
{
    VkPipelineShaderStageCreateInfo *shader_stages_info = pipeline_create_shader_stage_info(pipeline_info);

    VkPipelineVertexInputStateCreateInfo vertex_input_info = pipeline_create_vertex_input_info(pipeline_info);

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info = pipeline_create_input_assembly_info(pipeline_info);

    VkPipelineRasterizationStateCreateInfo rasterizer_info = pipeline_create_rasterization_info(pipeline_info);

    VkPipelineViewportStateCreateInfo viewport_info = pipeline_create_viewport_info(pipeline_info);

    VkPipelineMultisampleStateCreateInfo multisample_info = pipeline_create_multisample_info(pipeline_info);

    // TODO: Depth and stencil infos

    VkPipelineColorBlendStateCreateInfo color_blend_info = pipeline_create_color_blend_info(pipeline_info);

    VkPipelineDynamicStateCreateInfo dynamic_state_info = pipeline_create_dynamic_state_info(pipeline_info);

    VkPipelineLayoutCreateInfo pipeline_layout_info = pipeline_create_layout_info(pipeline_info);

    if (vkCreatePipelineLayout(backend->device.logical_device, &pipeline_layout_info, backend->vulkan_context.allocator, &pipeline->layout) != VK_SUCCESS)
    {
        DERROR("Could not create pipeline layout.");
        return D_ERROR;
    }

    VkGraphicsPipelineCreateInfo pipeline_create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    pipeline_create_info.stageCount = (u32)darray_size(&pipeline_info->shaders->shader_stages);
    pipeline_create_info.pStages = shader_stages_info;
    pipeline_create_info.pVertexInputState = &vertex_input_info;
    pipeline_create_info.pInputAssemblyState = &input_assembly_info;
    pipeline_create_info.pRasterizationState = &rasterizer_info;
    pipeline_create_info.pViewportState = &viewport_info;
    pipeline_create_info.pMultisampleState = &multisample_info;
    pipeline_create_info.pColorBlendState = &color_blend_info;
    pipeline_create_info.pDynamicState = &dynamic_state_info;
    pipeline_create_info.layout = pipeline->layout;
    pipeline_create_info.renderPass = pipeline_info->render_pass->render_pass_inner;
    pipeline_create_info.subpass = 0;
    pipeline_create_info.basePipelineHandle = NULL;

    if (vkCreateGraphicsPipelines(backend->device.logical_device, NULL, 1, &pipeline_create_info, backend->vulkan_context.allocator, &pipeline->pipeline_inner) != VK_SUCCESS)
    {
        DERROR("Unable to create graphics pipeline.");
        return D_ERROR;
    }
    pipeline->render_pass = *pipeline_info->render_pass;

    render_backend_destroy_pipeline_info(pipeline_info);

    return D_SUCCESS;
}

DResult render_backend_destroy_pipeline(RenderBackend *backend, RenderBackendPipeline *pipeline)
{
    vkDestroyPipeline(backend->device.logical_device, pipeline->pipeline_inner, backend->vulkan_context.allocator);
    vkDestroyPipelineLayout(backend->device.logical_device, pipeline->layout, backend->vulkan_context.allocator);
    return D_SUCCESS;
}

DResult render_backend_destroy_pipeline_info(PipelineInfo *pipeline_info)
{
    render_backend_destroy_pipeline_layout(pipeline_info->pipeline_layout);

    render_backend_destroy_color_blend_info(pipeline_info->color_blend_info);

    render_backend_destroy_multisample_info(pipeline_info->multisample_info);

    darray_destroy(pipeline_info->attribute_descriptions);
    darray_destroy(pipeline_info->binding_descriptions);
    darray_destroy(pipeline_info->dynamic_states);

    return D_SUCCESS;
}