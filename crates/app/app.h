#pragma once

#include "defines.h"

/* #include "../window/window.h"
#include "../render/render.h"
#include "../clock/clock.h"

#define MAX_APP_TITLE_LENGTH 100

typedef struct AppCreateInfo
{
  const char *title;
} AppInfo, AppCreateInfo;

typedef struct App
{
  AppInfo app_info;
  Window window; // TODO: Abstract to plugins (window will be a plugin)

  Renderer renderer;

  // TODO: Add systems so users can make their own runner / update / setup etc..
  DResult (*update)(struct App *app, f32 delta_time);
  DResult (*render)(struct App *app, f32 delta_time);
} App;

DAPI DResult app_create(App *app, AppCreateInfo *app_info);
DAPI DResult app_destroy(App app);

DAPI DResult app_run(App *app); */

DAPI void test(int x);