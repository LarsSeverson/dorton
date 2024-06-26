#include "render_backend_pipeline_layout.h"

RenderBackendPipelineLayout render_backend_create_default_pipeline_layout()
{
  RenderBackendPipelineLayout pipeline_layout = {0};
  darray_create(&pipeline_layout.descriptor_set_layouts, VkDescriptorSetLayout);
  darray_create(&pipeline_layout.push_constant_ranges, VkPushConstantRange);

  return pipeline_layout;
}

DResult render_backend_destroy_pipeline_layout(RenderBackendPipelineLayout *pipeline_layout)
{
  darray_destroy(&pipeline_layout->descriptor_set_layouts);
  darray_destroy(&pipeline_layout->push_constant_ranges);
  *pipeline_layout = (RenderBackendPipelineLayout){0};

  return D_SUCCESS;
}