#include "render_backend_pipeline_multisample.h"

#include <stdlib.h>

RenderBackendMultisampleInfo render_backend_create_default_multisample_info()
{
  RenderBackendMultisampleInfo multisampling_info = {0};
  multisampling_info.rasterization_samples = VK_SAMPLE_COUNT_1_BIT;
  multisampling_info.min_sample_shading = 1.0f;

  return multisampling_info;
}

DResult render_backend_destroy_multisample_info(RenderBackendMultisampleInfo *multisample_info)
{
  if (multisample_info->sample_mask != NULL)
  {
    free(multisample_info->sample_mask);
    multisample_info->sample_mask = NULL;
  }

  *multisample_info = (RenderBackendMultisampleInfo){0};

  return D_SUCCESS;
}