#include "render_backend_pipeline.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

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

DResult render_backend_create_pipeline(RenderBackend *backend, RenderBackendPipeline *pipeline, PipelineInfo *pipeline_info)
{
    VkPipelineVertexInputStateCreateInfo vertex_input_info = pipeline_create_vertex_input_info(pipeline_info);

    return D_SUCCESS;
}