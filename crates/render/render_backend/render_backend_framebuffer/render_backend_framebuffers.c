#include "render_backend_framebuffers.h"
#include "render_backend_framebuffer.h"

#include "../render_backend.h"

#include "logger.h"

DResult render_backend_create_framebuffers(RenderBackend *backend)
{
    u32 size = darray_size(&backend->swap_chain.images_views);
    darray_reserve(&backend->framebuffers.framebuffers_inner, RenderBackendFramebuffer, size);

    for (u32 i = 0; i < size; ++i)
    {
        VkImageView swap_chain_image_view = *(VkImageView *)darray_get(&backend->swap_chain.images_views, i);
        VkImageView attachments[] = {swap_chain_image_view};

        FramebufferInfo framebuffer_info = {0};
        framebuffer_info.attachment_count = 1;
        framebuffer_info.attachments = attachments;
        framebuffer_info.width = backend->swap_chain.extent.width;
        framebuffer_info.height = backend->swap_chain.extent.height;
        framebuffer_info.render_pass = backend->render_pass.render_pass_inner;

        RenderBackendFramebuffer *framebuffer = (RenderBackendFramebuffer *)darray_get(&backend->framebuffers.framebuffers_inner, i);

        if (render_backend_create_framebuffer(backend, framebuffer, &framebuffer_info) != D_SUCCESS)
        {
            DFATAL("Could not create framebuffers.");
            return D_FATAL;
        }
    }

    backend->framebuffers.framebuffers_size = size;

    return D_SUCCESS;
}

DResult render_backend_destroy_framebuffers(RenderBackend *backend)
{
    for (u32 i = 0; i < backend->framebuffers.framebuffers_size; ++i)
    {
        RenderBackendFramebuffer *framebuffer = (RenderBackendFramebuffer *)darray_get(&backend->framebuffers.framebuffers_inner, i);
        if (render_backend_destroy_framebuffer(backend, framebuffer) != D_SUCCESS)
        {
            DERROR("Could not destroy framebuffers.");
            return D_ERROR;
        }
    }

    darray_destroy(&backend->framebuffers.framebuffers_inner);
    
    return D_SUCCESS;
}