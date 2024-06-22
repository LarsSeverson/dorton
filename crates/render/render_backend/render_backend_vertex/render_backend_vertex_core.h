#pragma once

#include "render/render_backend/render_backend_core.h"

#include <cglm/cglm.h>

typedef enum VertexType
{
  VERTEX_TYPE_2D = 0,
  VERTEX_TYPE_3D = 1
} VertexType;

typedef enum VertexInputRate
{
  VERTEX_INPUT_RATE_VERTEX = 0,
  VERTEX_INPUT_RATE_INSTANCE = 1
} VertexInputRate;
