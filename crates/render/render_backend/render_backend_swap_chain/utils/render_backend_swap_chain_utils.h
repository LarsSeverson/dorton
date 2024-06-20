#pragma once

#include "window/window.h"
#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"

typedef struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilites;

  // VkSurfaceFormatKHR
  DArray formats;
  // VkPresentModeKHR
  DArray present_modes;
  
} SwapChainSupportDetails;

DResult swap_chain_support_details_create(SwapChainSupportDetails *details);
DResult swap_chain_support_details_destroy(SwapChainSupportDetails *details);

DResult query_swap_chain_support(SwapChainSupportDetails *details, const VkPhysicalDevice physical_device, VkSurfaceKHR surface);
u8 swap_chain_support_details_empty(SwapChainSupportDetails *details);

VkSurfaceFormatKHR choose_swap_chain_surface_format(DArray *formats, VkFormat format, VkColorSpaceKHR color_space);
VkPresentModeKHR choose_swap_chain_present_mode(DArray *present_modes, VkPresentModeKHR present_mode);
VkExtent2D choose_swap_chain_extent(VkSurfaceCapabilitiesKHR *capabilites, Window *window);