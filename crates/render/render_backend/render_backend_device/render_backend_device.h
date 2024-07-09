#pragma once

#include "render/render_backend/render_backend_core.h"
#include "render/render_backend/render_backend_command/render_backend_command_lib.h"

typedef struct RenderBackendDevice
{
  VkPhysicalDevice physical_device;
  VkDevice logical_device;

  u32 graphics_family;
  VkQueue graphics_queue;

  u32 present_family;
  VkQueue present_queue;

  u32 transfer_family;
  VkQueue transfer_queue;

  RenderBackendCommandPool graphics_command_pool;

} RenderBackendDevice;

struct RenderBackend;

DResult render_backend_create_device(struct RenderBackend *backend);
DResult render_backend_destroy_device(struct RenderBackend *backend);