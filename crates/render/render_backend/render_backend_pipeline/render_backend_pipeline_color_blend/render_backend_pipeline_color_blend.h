#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_pipeline/render_backend_pipeline_core.h"

typedef VkPipelineColorBlendAttachmentState ColorBlendAttachment;
typedef VkPipelineColorBlendStateCreateFlags BlendFlags;
typedef VkLogicOp LogicOperation;

typedef struct RenderBackendColorBlendInfo
{
  BlendFlags flags;

  dbool logic_operation_enable;
  LogicOperation logic_operation;

  // ColorBlendAttachment
  DArray attachments;

  f32 blend_constants[4];

} RenderBackendColorBlendInfo;

RenderBackendColorBlendInfo render_backend_create_default_color_blend_info();
DResult render_backend_destroy_color_blend_info(RenderBackendColorBlendInfo *color_blend_info);