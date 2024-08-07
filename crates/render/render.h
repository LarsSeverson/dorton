#pragma once

#include "window/window.h"

#include "./render_core.h"
#include "./render_backend/render_backend.h"

typedef struct RendererCreateInfo
{
  const char *app_title;
  Window *window;
} RendererCreateInfo;

typedef struct Renderer
{
  RenderBackend backend;
  Window *window;
} Renderer;

DResult renderer_create(Renderer *renderer, RendererCreateInfo *create_info);
DResult renderer_destroy(Renderer *renderer);

DResult renderer_draw(Renderer *renderer, RenderPacket packet);
DResult renderer_resize(Renderer *renderer, i32 width, i32 height);
