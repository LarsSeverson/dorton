#include "render_backend_render_pass.h"

#include "../render_backend.h"

#include "logger.h"

DResult render_backend_create_render_pass(RenderBackend *backend)
{
  VkAttachmentDescription color_attachment = {0};
  color_attachment.format = backend->swap_chain.swap_chain_image_format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref = {0};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription sub_pass = {0};
  sub_pass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  sub_pass.colorAttachmentCount = 1;
  sub_pass.pColorAttachments = &color_attachment_ref;

  VkRenderPassCreateInfo render_pass_create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
  render_pass_create_info.attachmentCount = 1;
  render_pass_create_info.pAttachments = &color_attachment;
  render_pass_create_info.subpassCount = 1;
  render_pass_create_info.pSubpasses = &sub_pass;

  if (vkCreateRenderPass(backend->device.logical_device, &render_pass_create_info, backend->vulkan_context.allocator, &backend->render_pass.render_pass_inner) != VK_SUCCESS)
  {
    DFATAL("Could not create render pass.");
    return D_FATAL;
  }

  return D_SUCCESS;
}

DResult render_backend_destroy_render_pass(RenderBackend *backend)
{
  vkDestroyRenderPass(backend->device.logical_device, backend->render_pass.render_pass_inner, backend->vulkan_context.allocator);
  return D_SUCCESS;
}