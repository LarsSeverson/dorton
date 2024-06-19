#pragma once

#include "render/render_types.h"
#include "darray/darray.h"

#include "../render_backend_shader/render_backend_shaders.h"

typedef u32 CullModeFlagBits;
typedef enum CullModeFlags
{
  CULL_MODE_NONE = 0x0,
  CULL_MODE_FRONT = 0x1,
  CULL_MODE_BACK = 0x2,
  CULL_MODE_FRONT_BACK = 0x3
} CullModeFlags;

typedef enum FrontFace
{
  FRONT_FACE_CLOCKWISE = 0,
  FRONT_FACE_COUNTER_CLOCKWISE = 1
} FrontFace;

typedef struct PipelineInfo
{
  VkViewport viewport;
  VkRect2D scissor;

  ShaderFlagBits shader_flags;
  RenderBackendShaders shaders;

  CullModeFlagBits cull_flags;
} PipelineInfo;

typedef struct RenderBackendPipeline
{

} RenderBackendPipeline;

struct RenderBackend;

DResult render_backend_create_pipeline(struct RenderBackend *backend, PipelineInfo *pipeline_info);