#pragma once

#include "render/render_backend/render_backend_core.h"
#include "render/render_backend/render_backend.h"

typedef struct QueueFamilyIndices
{
  u32 graphics_family;
  u32 present_family;

  u8 graphics_has_value;
  u8 present_has_value;
} QueueFamilyIndices;

DResult render_backend_pick_physical_device(RenderBackend *backend);
const char **get_physical_device_extensions();
const u32 get_physical_device_extensions_count();

DResult find_queue_families(QueueFamilyIndices *indices, const VkPhysicalDevice physical_device, VkSurfaceKHR surface);
u8 queue_family_indices_is_complete(QueueFamilyIndices *indices);