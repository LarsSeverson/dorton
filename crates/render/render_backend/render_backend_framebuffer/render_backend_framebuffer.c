#include "render_backend_framebuffer.h"

#include "logger.h"

DResult render_backend_create_framebuffer(RenderBackend *backend, RenderBackendFramebuffer *framebuffer, FramebufferInfo *framebuffer_info)
{
    VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    framebuffer_create_info.renderPass = framebuffer_info->render_pass;
    framebuffer_create_info.attachmentCount = framebuffer_info->attachment_count;
    framebuffer_create_info.pAttachments = framebuffer_info->attachments;
    framebuffer_create_info.width = framebuffer_info->width;
    framebuffer_create_info.height = framebuffer_info->height;
    framebuffer_create_info.layers = 1;


    if (vkCreateFramebuffer(backend->device.logical_device, &framebuffer_create_info, backend->vulkan_context.allocator, &framebuffer->framebuffer_inner) != VK_SUCCESS)
    {
        DERROR("Unable to create framebuffer.");
        return D_ERROR;
    }

    framebuffer->framebuffer_info = *framebuffer_info;

    return D_SUCCESS;
}

DResult render_backend_destroy_framebuffer(RenderBackend *backend, RenderBackendFramebuffer *framebuffer)
{
    vkDestroyFramebuffer(backend->device.logical_device, framebuffer->framebuffer_inner, backend->vulkan_context.allocator);
    
    return D_SUCCESS;
}