#pragma once

#include "render/render_backend/render_backend_pipeline/render_backend_pipeline_core.h"

typedef VkPipelineMultisampleStateCreateFlags MultisampleFlags;
typedef VkSampleCountFlagBits SampleCount;
typedef VkSampleMask SampleMask;

typedef struct RenderBackendMultisampleInfo
{
  MultisampleFlags flags;

  SampleCount rasterization_samples;

  dbool sample_shading_enable;

  f32 min_sample_shading;

  SampleMask *sample_mask;

  dbool alpha_to_coverage_enable;
  dbool alpha_to_one_enable;

} RenderBackendMultisampleInfo;

RenderBackendMultisampleInfo render_backend_create_default_multisample_info();