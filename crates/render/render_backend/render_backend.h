#pragma once

#include "./render_backend_core.h"

#include "window/window.h"
#include "darray/darray.h"

#include "./render_backend_component/render_backend_components.h"
#include "./render_backend_device/render_backend_device.h"
#include "./render_backend_swap_chain/render_backend_swap_chain.h"
#include "./render_backend_framebuffer/render_backend_framebuffers.h"
#include "./render_backend_sync/render_backend_fence/render_backend_fences.h"
#include "./render_backend_sync/render_backend_semaphore/render_backend_semaphores.h"
#include "./render_backend_command/render_backend_command_pool/render_backend_command_pool.h"

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

  RenderBackendFences in_flight_fences;
  RenderBackendSemaphores image_available_semaphores;
  RenderBackendSemaphores render_finished_semaphores;

  VulkanContext vulkan_context;

  u32 current_frame;
  dbool resized;

  RenderBackendCommandPool transfer_command_pool;

  RenderBackendComponents components;

} RenderBackend;

DResult render_backend_create(RenderBackend *backend, RenderBackendCreateInfo *create_info);
DResult render_backend_destroy(RenderBackend *backend);

DResult render_backend_draw(RenderBackend *backend, RenderPacket packet);

DResult render_backend_begin_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet);
DResult render_backend_process_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet);
DResult render_backend_draw_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet);
DResult render_backend_end_frame(RenderBackend *backend, RenderBackendDrawPacket *draw_packet);

DResult render_backend_add_component(RenderBackend *backend, RenderBackendComponentInfo *component_info);
DResult render_backend_add_empty_component(RenderBackend *backend);

DResult render_backend_resize(RenderBackend *backend, i32 width, i32 height);
