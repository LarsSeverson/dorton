#include "render_backend_pipeline_rasterizer.h"

RenderBackendRasterizerInfo render_backend_create_default_rasterizer_info()
{
    RenderBackendRasterizerInfo rasterizer_info = {0};
    rasterizer_info.cull_mode = VK_CULL_MODE_BACK_BIT;
    rasterizer_info.front_face = VK_FRONT_FACE_CLOCKWISE;
    rasterizer_info.polygon_mode = VK_POLYGON_MODE_FILL;

    return rasterizer_info;
}
