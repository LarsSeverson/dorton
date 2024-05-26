#pragma once

#include "render_types.inl"

#include "./render_backend/render_backend.h"

typedef struct Renderer
{
  RenderBackend backend;
} Renderer;

DResult renderer_create(Renderer *renderer, const char* app_title);
DResult renderer_destroy(Renderer renderer);

DResult renderer_draw(Renderer* renderer, RenderPacket packet);

