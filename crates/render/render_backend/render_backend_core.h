#pragma once

#include "defines.h"

#include "darray/darray.h"

#include "render/render_core.h"

#include <vulkan/vulkan.h>

typedef struct VulkanContext
{
  VkInstance instance;
  VkSurfaceKHR surface;

  VkAllocationCallbacks *allocator;
  VkDebugUtilsMessengerEXT debug_messenger;
} VulkanContext;

typedef struct RenderBackendDrawPacket
{
  u32 image_index;
  u32 current_frame;

  // RenderBackendCommandBuffer
  DArray draw_command_buffers;

} RenderBackendDrawPacket;