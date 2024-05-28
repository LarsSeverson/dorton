#pragma once

#include "../render_types.inl"

typedef struct RenderBackendCreateInfo
{
  const char* app_name;
  VulkanContext vulkan_context;
} RenderBackendCreateInfo;

typedef struct RenderBackend
{
  VulkanContext vulkan_context;
} RenderBackend;

DResult render_backend_create(RenderBackend *backend, RenderBackendCreateInfo* create_info);
DResult render_backend_destroy(RenderBackend *backend);

DResult render_backend_begin_frame(RenderBackend* backend, f32 delta_time);
DResult render_backend_end_frame(RenderBackend* backend, f32 delta_time);