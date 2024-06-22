#pragma once

#include "render/render_backend/render_backend_core.h"
#include "render/render_backend/render_backend_shader/render_backend_shader_lib.h"

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