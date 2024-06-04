#pragma once

#include "../render_types.h"

#include "window/window.h"

#include "./render_backend_device/render_backend_device.h"
#include "./render_backend_swap_chain/render_backend_swap_chain.h"
#include "./render_backend_render_pass/render_backend_render_pass.h"

typedef struct RenderBackendCreateInfo
{
  const char *app_title;
  Window *window;
} RenderBackendCreateInfo;

typedef struct RenderBackend
{
  const char *app_title;
  Window *window;

  RenderBackendDevice device;
  RenderBackendSwapChain swap_chain;
  RenderBackendRenderPass render_pass;

  VulkanContext vulkan_context;
} RenderBackend;

DResult render_backend_create(RenderBackend *backend, RenderBackendCreateInfo *create_info);
DResult render_backend_destroy(RenderBackend *backend);

DResult render_backend_begin_frame(RenderBackend *backend, f32 delta_time);
DResult render_backend_end_frame(RenderBackend *backend, f32 delta_time);