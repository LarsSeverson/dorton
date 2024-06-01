#include "render_backend_swap_chain.h"

#include "logger.h"

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

DResult query_swap_chain_support(SwapChainSupportDetails *details, const VkPhysicalDevice *physical_device, VkSurfaceKHR *surface)
{
  if (details == NULL || surface == NULL || physical_device == NULL)
  {
    return D_ERROR;
  }

  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*physical_device, *surface, &details->capabilites) != D_SUCCESS)
  {
    return D_ERROR;
  }

  u32 format_count;
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(*physical_device, *surface, &format_count, NULL) != D_SUCCESS)
  {
    DERROR("Error getting physical device surface formats.");
    return D_ERROR;
  }

  if (format_count != 0)
  {
    darray_resize(&details->formats, format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(*physical_device, *surface, &format_count, (VkSurfaceFormatKHR *)darray_data(&details->formats));
  }

  u32 present_mode_count;
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(*physical_device, *surface, &present_mode_count, NULL) != D_SUCCESS)
  {
    DERROR("Error getting physical device present modes.");
    return D_ERROR;
  }

  if (present_mode_count != 0)
  {
    darray_resize(&details->present_modes, present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(*physical_device, *surface, &present_mode_count, (VkPresentModeKHR *)darray_data(&details->present_modes));
  }

  return D_SUCCESS;
}

u8 swap_chain_support_details_empty(SwapChainSupportDetails *details)
{
  return darray_empty(&details->formats) && darray_empty(&details->present_modes);
}