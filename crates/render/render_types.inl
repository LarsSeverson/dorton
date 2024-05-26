#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>

typedef struct RenderPacket
{
  f32 delta_time;
} RenderPacket;

typedef struct VulkanContext
{
  VkInstance instance;
  VkAllocationCallbacks *allocation_callbacks;
} VulkanContext;