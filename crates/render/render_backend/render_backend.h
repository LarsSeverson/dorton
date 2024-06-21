#pragma once

#include "./render_backend_core.h"

#include "window/window.h"

#include "./render_backend_device/render_backend_device.h"
#include "./render_backend_swap_chain/render_backend_swap_chain.h"
#include "./render_backend_render_pass/render_backend_render_pass.h"
#include "./render_backend_command/render_backend_command_pool.h"
#include "./render_backend_command/render_backend_command_buffer/render_backend_command_buffers.h"
#include "./render_backend_framebuffer/render_backend_framebuffers.h"
#include "./render_backend_sync/render_backend_fence/render_backend_fences.h"
#include "./render_backend_sync/render_backend_semaphore/render_backend_semaphores.h"
#include "./render_backend_vertex/render_backend_vertex_lib.h"

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
  RenderBackendCommandPool command_pool;
  RenderBackendCommandBuffers command_buffers;
  RenderBackendFramebuffers framebuffers;
  RenderBackendFences fences;
  RenderBackendSemaphores semaphores;

  VulkanContext vulkan_context;
} RenderBackend;

DResult render_backend_create(RenderBackend *backend, RenderBackendCreateInfo *create_info);
DResult render_backend_destroy(RenderBackend *backend);

DResult render_backend_begin_frame(RenderBackend *backend, f32 delta_time);
DResult render_backend_end_frame(RenderBackend *backend, f32 delta_time);

DResult render_backend_resize(RenderBackend *backend, i32 width, i32 height);
