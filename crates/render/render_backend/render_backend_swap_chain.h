#pragma once

#include "./render_backend.h"

#include "darray/darray.h"

typedef struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilites;

  DArray formats;       // VkSurfaceFormatKHR
  DArray present_modes; // VkPresentModeKHR
} SwapChainSupportDetails;

DResult swap_chain_support_details_create(SwapChainSupportDetails *details);
DResult swap_chain_support_details_destroy(SwapChainSupportDetails *details);

DResult query_swap_chain_support(SwapChainSupportDetails *details, const VkPhysicalDevice *physical_device, VkSurfaceKHR *surface);
u8 swap_chain_support_details_empty(SwapChainSupportDetails *details);