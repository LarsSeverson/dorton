#pragma once

#include "render/render_backend/render_backend_core.h"

struct RenderBackend;

typedef struct RenderBackendDevice
{
  VkPhysicalDevice physical_device;
  VkDevice logical_device;

  VkQueue graphics_queue;
  VkQueue present_queue;
} RenderBackendDevice;

DResult render_backend_create_device(struct RenderBackend *backend);
DResult render_backend_destroy_device(struct RenderBackend *backend);