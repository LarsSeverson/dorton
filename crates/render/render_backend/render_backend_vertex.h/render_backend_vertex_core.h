#pragma once

#include "render/render_types.h"

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

typedef struct VertexInputBindingDescription
{
  u32 binding;
  u32 stride;
  VertexInputRate input_rate;
} VertexInputBindingDescription;

typedef struct VertexInputAttributeDescription
{
  u32 binding;
  u32 location;
  u32 offset;
  u32 format;
} VertexInputAttributeDescription;