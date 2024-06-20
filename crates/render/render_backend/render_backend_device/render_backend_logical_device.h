#pragma once

#include "render/render_backend/render_backend_core.h"
#include "render/render_backend/render_backend.h"

DResult render_backend_create_logical_device(RenderBackend *backend);
DResult render_backend_destroy_logical_device(RenderBackend *backend);