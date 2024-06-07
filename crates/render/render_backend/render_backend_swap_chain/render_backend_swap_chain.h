#pragma once

#include "render/render_types.h"
#include "darray/darray.h"

// "../render_backend.h"
struct RenderBackend;

typedef struct RenderBackendSwapChain
{
  VkSwapchainKHR swap_chain_inner;

  // VkImage
  DArray swap_chain_images;
  // VkImageView
  DArray swap_chain_image_views;
  // VkFrameBuffer
  DArray swap_chain_frame_buffer;

  VkFormat swap_chain_image_format;
  VkExtent2D swap_chain_extent;

  u32 swap_chain_images_count;

} RenderBackendSwapChain;

DResult render_backend_create_swap_chain(struct RenderBackend *backend);
DResult render_backend_destroy_swap_chain(struct RenderBackend *backend);