#pragma once

#include "render/render_types.h"
#include "darray/darray.h"

// "../render_backend.h"
struct RenderBackend;

typedef struct RenderBackendSwapChain
{
  VkSwapchainKHR swap_chain_inner;

  // VkImage
  DArray images;
  // VkImageView
  DArray images_views;

  VkFormat image_format;
  VkExtent2D extent;

  u32 images_count;
  u8 max_frames_in_flight;

} RenderBackendSwapChain;

DResult render_backend_create_swap_chain(struct RenderBackend *backend);
DResult render_backend_destroy_swap_chain(struct RenderBackend *backend);
DResult render_backend_recreate_swap_chain(struct RenderBackend *backend);