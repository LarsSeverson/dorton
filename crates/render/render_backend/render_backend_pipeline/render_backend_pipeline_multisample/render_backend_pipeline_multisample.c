#include "render_backend_pipeline_multisample.h"

RenderBackendMultisampleInfo render_backend_create_default_multisample_info()
{
  RenderBackendMultisampleInfo multisampling_info = {0};
  multisampling_info.rasterization_samples = VK_SAMPLE_COUNT_1_BIT;
  multisampling_info.min_sample_shading = 1.0f;

  return multisampling_info;
}