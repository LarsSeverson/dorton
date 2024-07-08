#pragma once

#include "render/render_backend/render_backend.h"

DResult render_backend_predraw_process_result(RenderBackend *backend, VkResult result);
DResult render_backend_postdraw_process_result(RenderBackend *backend, VkResult result);
