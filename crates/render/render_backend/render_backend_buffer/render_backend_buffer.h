#pragma once

#include "render/render_backend/render_backend_core.h"
#include "render/render_backend/render_backend_command/render_backend_command_pool/render_backend_command_pool.h"

typedef struct BufferInfo
{
  VkBufferUsageFlags usage;
  VkMemoryPropertyFlags properties;
  VkSharingMode sharing_mode;

  u64 size;
  u64 offset;
} BufferInfo;

typedef struct RenderBackendBuffer
{
  VkBuffer buffer_inner;
  VkDeviceMemory memory;

  u64 size;
  u64 offset;
} RenderBackendBuffer;

struct RenderBackend;

DResult render_backend_create_buffer(struct RenderBackend *backend, RenderBackendBuffer *buffer, BufferInfo *buffer_info);
DResult render_backend_destroy_buffer(struct RenderBackend *backend, RenderBackendBuffer *buffer);

DResult render_backend_copy_buffer(struct RenderBackend *backend, RenderBackendBuffer *source, RenderBackendBuffer *destination, u64 size);