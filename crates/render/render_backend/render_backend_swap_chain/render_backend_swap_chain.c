#include "render_backend_swap_chain.h"

#include "logger.h"

#include "./utils/render_backend_swap_chain_utils.h"

#include "render/render_backend/render_backend.h"
#include "render/render_backend/render_backend_device/render_backend_physical_device.h"
#include "render/render_backend/utils/render_backend_utils.h"

#define MAX_FRAMES_IN_FLIGHT 2

DResult swap_chain_image_views_create(RenderBackendSwapChain *swap_chain, VkDevice logical_device, VkAllocationCallbacks *allocator)
{
    darray_reserve(&swap_chain->images_views, VkImageView, darray_size(&swap_chain->images));

    for (u32 i = 0; i < darray_size(&swap_chain->images_views); ++i)
    {
        VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        image_view_create_info.image = *(VkImage *)darray_get(&swap_chain->images, i);
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = swap_chain->image_format;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(logical_device, &image_view_create_info, allocator, (VkImageView *)darray_get(&swap_chain->images_views, i)) != VK_SUCCESS)
        {
            DFATAL("Swap chain image views could not be created.");
            return D_FATAL;
        }
    }

    return D_SUCCESS;
}

DResult render_backend_create_swap_chain(RenderBackend *backend)
{
    SwapChainSupportDetails swap_chain_support_details;
    swap_chain_support_details_create(&swap_chain_support_details);
    query_swap_chain_support(&swap_chain_support_details, backend->device.physical_device, backend->vulkan_context.surface);

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
    find_queue_families(&indices, backend->device.physical_device, backend->vulkan_context.surface);
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

    if (vkGetSwapchainImagesKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, &image_count, NULL) != D_SUCCESS)
    {
        DERROR("Error getting swap chain image count.");
        return D_ERROR;
    }

    darray_reserve(&backend->swap_chain.images, VkImage, image_count);

    if (vkGetSwapchainImagesKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, &image_count, (VkImage *)darray_data(&backend->swap_chain.images)) != D_SUCCESS)
    {
        DERROR("Error getting swap chain images.");
        return D_ERROR;
    }

    backend->swap_chain.images_count = image_count;
    backend->swap_chain.image_format = swap_chain_surface_format.format;
    backend->swap_chain.extent = swap_chain_extent;
    backend->swap_chain.max_frames_in_flight = MAX_FRAMES_IN_FLIGHT;

    return swap_chain_image_views_create(&backend->swap_chain, backend->device.logical_device, backend->vulkan_context.allocator);
}

DResult render_backend_destroy_swap_chain(RenderBackend *backend)
{
    for (u32 i = 0; i < darray_size(&backend->swap_chain.images_views); ++i)
    {
        vkDestroyImageView(backend->device.logical_device, *(VkImageView *)darray_get(&backend->swap_chain.images_views, i), backend->vulkan_context.allocator);
    }

    vkDestroySwapchainKHR(backend->device.logical_device, backend->swap_chain.swap_chain_inner, backend->vulkan_context.allocator);

    if (darray_destroy(&backend->swap_chain.images_views) != D_SUCCESS)
    {
        DERROR("Error destroying swap chain image views.");
        return D_ERROR;
    }

    if (darray_destroy(&backend->swap_chain.images) != D_SUCCESS)
    {
        DERROR("Error destroying swap chain images.");
        return D_ERROR;
    }

    return D_SUCCESS;
}

DResult render_backend_recreate_swap_chain(RenderBackend *backend)
{
    i32 width = 0, height = 0;
    window_get_frame_buffer_size(backend->window, &width, &height);
    while (width == 0 || height == 0)
    {
        window_get_frame_buffer_size(backend->window, &width, &height);
        glfwWaitEvents();
    }

    if (vk_is_success(vkDeviceWaitIdle(backend->device.logical_device)) != D_SUCCESS)
    {
        DERROR("Could not wait for device to idle on swapchain recreate.");
        return D_ERROR;
    }
    
    if (render_backend_destroy_swap_chain(backend) != D_SUCCESS)
    {
        DERROR("Could not destroy swapchain on recreate.");
        return D_ERROR;
    }

    if (render_backend_create_swap_chain(backend) != D_SUCCESS)
    {
        DERROR("Could not create swapchain on recreate.");
        return D_ERROR;
    }

    return D_SUCCESS;
}