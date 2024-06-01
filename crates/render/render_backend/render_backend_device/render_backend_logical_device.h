#pragma once

#include "../../render_types.inl"

#include "../render_backend.h"

DResult render_backend_create_logical_device(RenderBackend *backend);
DResult render_backend_destroy_logical_device(RenderBackend *backend);