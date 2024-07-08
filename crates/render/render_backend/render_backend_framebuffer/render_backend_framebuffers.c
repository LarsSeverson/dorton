#include "render_backend_framebuffers.h"
#include "render_backend_framebuffer.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_framebuffers(RenderBackend *backend, RenderBackendFramebuffers *framebuffers, FramebuffersInfo *framebuffers_info)
{
  u32 size = backend->swap_chain.max_frames_in_flight;

  darray_reserve(&framebuffers->framebuffers_inner, RenderBackendFramebuffer, size);

  for (u32 i = 0; i < size; ++i)
  {
    VkImageView swap_chain_image_view = *(VkImageView *)darray_get(&backend->swap_chain.images_views, i);
    VkImageView attachments[] = {swap_chain_image_view};

    FramebufferInfo framebuffer_info = {0};
    framebuffer_info.attachment_count = 1;
    framebuffer_info.attachments = attachments;
    framebuffer_info.width = backend->swap_chain.extent.width;
    framebuffer_info.height = backend->swap_chain.extent.height;
    framebuffer_info.render_pass = &framebuffers_info->render_pass->render_pass_inner;

    RenderBackendFramebuffer *framebuffer = (RenderBackendFramebuffer *)darray_get(&framebuffers->framebuffers_inner, i);

    if (render_backend_create_framebuffer(backend, framebuffer, &framebuffer_info) != D_SUCCESS)
    {
      DFATAL("Could not create framebuffers.");
      return D_FATAL;
    }
  }

  framebuffers->size = size;
  framebuffers->render_pass = framebuffers_info->render_pass;

  return D_SUCCESS;
}

DResult render_backend_destroy_framebuffers(RenderBackend *backend, RenderBackendFramebuffers *framebuffers)
{
  for (u32 i = 0; i < framebuffers->size; ++i)
  {
    RenderBackendFramebuffer *framebuffer = (RenderBackendFramebuffer *)darray_get(&framebuffers->framebuffers_inner, i);
    if (render_backend_destroy_framebuffer(backend, framebuffer) != D_SUCCESS)
    {
      DERROR("Could not destroy framebuffers.");
      return D_ERROR;
    }
  }

  darray_destroy(&framebuffers->framebuffers_inner);

  return D_SUCCESS;
}

DResult render_backend_recreate_framebuffers(RenderBackend *backend, RenderBackendFramebuffers *framebuffers)
{
  if (render_backend_destroy_framebuffers(backend, framebuffers) != D_SUCCESS)
  {
    return D_ERROR;
  }

  FramebuffersInfo framebuffers_info = {0};
  framebuffers_info.render_pass = framebuffers->render_pass;

  if (render_backend_create_framebuffers(backend, framebuffers, &framebuffers_info) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

RenderBackendFramebuffer *framebuffers_get(RenderBackendFramebuffers *framebuffers, u32 index)
{
  RenderBackendFramebuffer *framebuffer = (RenderBackendFramebuffer *)darray_get(&framebuffers->framebuffers_inner, index);
  return framebuffer;
}