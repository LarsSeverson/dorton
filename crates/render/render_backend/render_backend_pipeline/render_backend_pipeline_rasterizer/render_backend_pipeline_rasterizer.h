#pragma once

#include "render/render_backend/render_backend_pipeline/render_backend_pipeline_core.h"

typedef struct RenderBackendRasterizerInfo
{
    VkCullModeFlagBits cull_mode;
    VkFrontFace front_face;
    VkPolygonMode polygon_mode;
} RenderBackendRasterizerInfo;

RenderBackendRasterizerInfo render_backend_create_default_rasterizer_info();
