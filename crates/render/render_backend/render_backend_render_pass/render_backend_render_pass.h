#pragma once

#include "render/render_backend/render_backend_core.h"

typedef struct RenderPassInfo
{
  // TODO:
} RenderPassInfo;

typedef struct RenderBackendRenderPass
{
  VkRenderPass render_pass_inner;
  
} RenderBackendRenderPass;

struct RenderBackend;

RenderPassInfo create_default_render_pass_info(struct RenderBackend *backend);

DResult render_backend_create_render_pass(struct RenderBackend *backend, RenderBackendRenderPass *render_pass, RenderPassInfo *render_pass_info);
DResult render_backend_destroy_render_pass(struct RenderBackend *backend, RenderBackendRenderPass *render_pass);
