#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_pipeline/render_backend_pipeline_core.h"

typedef struct RenderBackendPipelineLayout
{
  VkPipelineLayoutCreateFlags flags;

  // VkDescriptorSetLayout
  DArray descriptor_set_layouts;
  // VkPushConstantRange
  DArray push_constant_ranges;

} RenderBackendPipelineLayout;

RenderBackendPipelineLayout render_backend_create_default_pipeline_layout();
DResult render_backend_destroy_pipeline_layout(RenderBackendPipelineLayout *pipeline_layout);
