#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_pipeline/render_backend_pipeline_core.h"

typedef struct RenderBackendColorBlendInfo
{
  VkPipelineColorBlendStateCreateFlags flags;

  dbool logic_operation_enable;
  VkLogicOp logic_operation;

  // VkPipelineColorBlendAttachmentState
  DArray attachments;

  f32 blend_constants[4];

} RenderBackendColorBlendInfo;

RenderBackendColorBlendInfo render_backend_create_default_color_blend_info();
DResult render_backend_destroy_color_blend_info(RenderBackendColorBlendInfo *color_blend_info);