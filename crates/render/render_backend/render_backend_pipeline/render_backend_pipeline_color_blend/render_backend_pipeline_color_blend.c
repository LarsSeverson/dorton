#include "render_backend_pipeline_color_blend.h"

// TODO: Learn more about color blending then come back and change some of this

ColorBlendAttachment render_backend_create_default_color_blend_attachment()
{
  ColorBlendAttachment color_blend_attachment = {0};
  color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  return color_blend_attachment;
}

RenderBackendColorBlendInfo render_backend_create_default_color_blend_info()
{
  ColorBlendAttachment color_blend_attachment = render_backend_create_default_color_blend_attachment();
  
  RenderBackendColorBlendInfo color_blend_info = {0};

  darray_reserve(&color_blend_info.attachments, ColorBlendAttachment, 1);
  darray_set(&color_blend_info.attachments, color_blend_attachment, 0);

  return color_blend_info;
}

DResult render_backend_destroy_color_blend_info(RenderBackendColorBlendInfo *color_blend_info)
{
  darray_destroy(&color_blend_info->attachments);

  return D_SUCCESS;
}