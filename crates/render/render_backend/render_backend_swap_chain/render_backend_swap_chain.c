#include "render_backend_swap_chain.h"

#include "../render_backend.h"
#include "../render_backend_device/render_backend_physical_device.h"

#include "utils/render_backend_swap_chain_utils.h"
#include "logger.h"

DResult render_backend_create_swap_chain(struct RenderBackend *backend)
{
    SwapChainSupportDetails swap_chain_support_details;
    swap_chain_support_details_create(&swap_chain_support_details);

    query_swap_chain_support(&swap_chain_support_details, &backend->device.physical_device, &backend->vulkan_context.surface);

    // TODO: Make the format and present mode based on what the user configures
    VkSurfaceFormatKHR swap_chain_surface_format = choose_swap_chain_surface_format(&swap_chain_support_details.formats, VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
    VkPresentModeKHR swap_chain_present_mode = choose_swap_chain_present_mode(&swap_chain_support_details.present_modes, VK_PRESENT_MODE_MAILBOX_KHR);
    VkExtent2D swap_chain_extent = choose_swap_chain_extent(&swap_chain_support_details.capabilites, backend->window);

    u32 image_count = swap_chain_support_details.capabilites.minImageCount + 1;
    if (swap_chain_support_details.capabilites.maxImageCount > 0 && image_count > swap_chain_support_details.capabilites.maxImageCount)
    {
        image_count = swap_chain_support_details.capabilites.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swap_chain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    swap_chain_create_info.surface = backend->vulkan_context.surface;
    swap_chain_create_info.minImageCount = image_count;
    swap_chain_create_info.imageFormat = swap_chain_surface_format.format;
    swap_chain_create_info.imageColorSpace = swap_chain_surface_format.colorSpace;
    swap_chain_create_info.imageExtent = swap_chain_extent;
    swap_chain_create_info.imageArrayLayers = 1;
    swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = {0};
    find_queue_families(&indices, &backend->device.physical_device, &backend->vulkan_context.surface);
    u32 queue_family_indices[] = {indices.graphics_family, indices.present_family};

    if (indices.graphics_family != indices.present_family)
    {
        swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swap_chain_create_info.queueFamilyIndexCount = 2;
        swap_chain_create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else
    {
        swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swap_chain_create_info.queueFamilyIndexCount = 0;
        swap_chain_create_info.pQueueFamilyIndices = NULL;
    }

    swap_chain_create_info.preTransform = swap_chain_support_details.capabilites.currentTransform;
    swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swap_chain_create_info.presentMode = swap_chain_present_mode;
    swap_chain_create_info.clipped = VK_TRUE;
    swap_chain_create_info.oldSwapchain = NULL;

    VkResult result = vkCreateSwapchainKHR(backend->device.logical_device, &swap_chain_create_info, backend->vulkan_context.allocator, &backend->swap_chain.swap_chain_inner);

    swap_chain_support_details_destroy(&swap_chain_support_details);

    if (result != D_SUCCESS)
    {
        DFATAL("Swap chain could not be created.");
        return D_FATAL;
    }

    vkGetSwapchainImagesKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, &image_count, NULL);
    darray_reserve(&backend->swap_chain.swap_chain_images, VkImage, image_count);
    vkGetSwapchainImagesKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, &image_count, (VkImage *)darray_data(&backend->swap_chain.swap_chain_images));

    backend->swap_chain.swap_chain_image_format = swap_chain_surface_format.format;
    backend->swap_chain.swap_chain_extent = swap_chain_extent;

    return D_SUCCESS;
}

DResult render_backend_destroy_swap_chain(struct RenderBackend *backend)
{
    vkDestroySwapchainKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, backend->vulkan_context.allocator);
    return darray_destroy(&backend->swap_chain.swap_chain_images);
}