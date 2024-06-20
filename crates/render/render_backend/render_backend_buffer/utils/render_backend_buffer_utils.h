#pragma once

#include "render/render_backend/render_backend.h"

u32 find_buffer_memory_type(RenderBackend *backend, u32 type_filter, VkMemoryPropertyFlags properties);