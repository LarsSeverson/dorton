#pragma once

#include "render/render_backend/render_backend_pipeline/render_backend_pipeline_core.h"

typedef struct RenderBackendMultisampleInfo
{
  VkPipelineMultisampleStateCreateFlags flags;

  VkSampleCountFlagBits rasterization_samples;

  dbool sample_shading_enable;

  f32 min_sample_shading;

  VkSampleMask *sample_mask;

  dbool alpha_to_coverage_enable;
  dbool alpha_to_one_enable;

} RenderBackendMultisampleInfo;

RenderBackendMultisampleInfo render_backend_create_default_multisample_info();
DResult render_backend_destroy_multisample_info(RenderBackendMultisampleInfo *multisample_info);