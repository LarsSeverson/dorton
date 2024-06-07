#include "render_backend_swap_chain_utils.h"

#include "logger.h"

#include <stdint.h>

DResult swap_chain_support_details_create(SwapChainSupportDetails *details)
{
  if (darray_create(&details->formats, VkSurfaceFormatKHR) != D_SUCCESS)
  {
    return D_ERROR;
  }
  if (darray_create(&details->present_modes, VkPresentModeKHR) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult swap_chain_support_details_destroy(SwapChainSupportDetails *details)
{
  if (darray_destroy(&details->formats) != D_SUCCESS)
  {
    return D_ERROR;
  }
  if (darray_destroy(&details->present_modes) != D_SUCCESS)
  {
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult query_swap_chain_support(SwapChainSupportDetails *details, const VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
  if (details == NULL)
  {
    return D_ERROR;
  }

  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details->capabilites) != D_SUCCESS)
  {
    return D_ERROR;
  }

  u32 format_count;
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, NULL) != D_SUCCESS)
  {
    DERROR("Error getting physical device surface formats.");
    return D_ERROR;
  }

  if (format_count != 0)
  {
    darray_resize(&details->formats, format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, (VkSurfaceFormatKHR *)darray_data(&details->formats));
  }

  u32 present_mode_count;
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, NULL) != D_SUCCESS)
  {
    DERROR("Error getting physical device present modes.");
    return D_ERROR;
  }

  if (present_mode_count != 0)
  {
    darray_resize(&details->present_modes, present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, (VkPresentModeKHR *)darray_data(&details->present_modes));
  }

  return D_SUCCESS;
}

u8 swap_chain_support_details_empty(SwapChainSupportDetails *details)
{
  return darray_empty(&details->formats) && darray_empty(&details->present_modes);
}

VkPresentModeKHR choose_swap_chain_present_mode(DArray *present_modes, VkPresentModeKHR present_mode)
{
  for (u32 i = 0; i < darray_size(present_modes); ++i)
  {
    VkPresentModeKHR available_present_mode = *(VkPresentModeKHR *)darray_get(present_modes, i);
    if (available_present_mode == present_mode)
    {
      return available_present_mode;
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR choose_swap_chain_surface_format(DArray *formats, VkFormat format, VkColorSpaceKHR color_space)
{
  for (u32 i = 0; i < darray_size(formats); ++i)
  {
    VkSurfaceFormatKHR available_format = *(VkSurfaceFormatKHR *)darray_get(formats, i);
    if (available_format.format == format && available_format.colorSpace == color_space)
    {
      return available_format;
    }
  }
  return *(VkSurfaceFormatKHR *)darray_get(formats, 0);
}

VkExtent2D choose_swap_chain_extent(VkSurfaceCapabilitiesKHR *capabilites, Window *window)
{
  if (capabilites->currentExtent.width != UINT32_MAX)
  {
    return capabilites->currentExtent;
  }

  i32 width = 0;
  i32 height = 0;
  window_get_frame_buffer_size(window, &width, &height);

  VkExtent2D actual_extent = {width, height};
  actual_extent.width = DCLAMP(actual_extent.width, capabilites->minImageExtent.width, capabilites->maxImageExtent.width);
  actual_extent.height = DCLAMP(actual_extent.height, capabilites->minImageExtent.height, capabilites->maxImageExtent.height);

  return actual_extent;
}