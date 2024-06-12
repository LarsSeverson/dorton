#include "app.h"

#include <stdio.h>

#include "../logger/logger.h"

static b8 initialized = false; // TODO: [Temp] no more than 1 app at a time

DResult update(App *app, f32 delta_time);
DResult render(App *app, f32 delta_time);

DResult app_create(App *app, AppCreateInfo *app_info)
{
  DINFO("TEST");
  if (initialized)
  {
    DFATAL("Only 1 app can exist.");
    return D_ERROR;
  }

  *app = (App){0}; // Initializing app properly

  if (init_logging() != D_SUCCESS)
  {
    DERROR("Logging initialization failure.");
    return D_ERROR;
  }

  WindowCreateInfo window_create_info;
  window_create_info.title = app_info->title;
  window_create_info.height = 720;
  window_create_info.width = 1250;

  if (window_create(app, &app->window, &window_create_info) != D_SUCCESS)
  {
    DFATAL("Error creating window");
    return D_ERROR;
  }

  RendererCreateInfo renderer_create_info;
  renderer_create_info.app_title = app_info->title;
  renderer_create_info.window = &app->window;

  if (renderer_create(&app->renderer, &renderer_create_info) != D_SUCCESS)
  {
    DFATAL("App could not create renderer.");
    return D_FATAL;
  }

  app->update = update;
  app->render = render;

  initialized = true;

  DINFO("App created successfully.");
  return D_SUCCESS;
}

DResult update(App *app, f32 delta_time)
{
  return D_SUCCESS;
}

DResult render(App *app, f32 delta_time)
{
  return D_SUCCESS;
}

DResult app_run(App *app)
{
  u64 last_time = clock_get_nanoseconds();
  f64 FPS = 60.0;
  f64 ns = 1000000000.0 / FPS;
  f64 delta = 0.0;
  u16 frames = 0;
  f64 time = clock_get_milliseconds();

  while (window_running(&app->window))
  {
    u64 now = clock_get_nanoseconds();

    delta += (now - last_time) / ns;
    last_time = now;

    window_poll_events(&app->window);

    if (delta >= 1.0)
    {

      if (app->update(app, (f32)(delta * (1000.0 / FPS))) != D_SUCCESS)
      {
        DFATAL("App failed to update.");
        break;
      }

      if (app->render(app, (f32)(delta * (1000.0 / FPS))) != D_SUCCESS)
      {
        DFATAL("App failed to render.");
        break;
      }

      frames++;
      delta--;

      if (clock_get_milliseconds() - time >= 1000)
      {
        time += 1000;
        frames = 0;
      }
    }
  }

  return D_SUCCESS;
}

DResult app_destroy(App *app)
{
  if (renderer_destroy(&app->renderer) != D_SUCCESS)
  {
    DERROR("App could not destroy renderer.");
  }

  DINFO("Renderer destroyed successfully.");

  if (window_destroy(&app->window) != D_SUCCESS)
  {
    DERROR("App could not destroy window.");
  }

  DINFO("Window destroyed successfully.");

  DINFO("App destroyed successfully.");

  return D_SUCCESS;
}

DResult app_resize(App *app, WindowEvent window_resize_event)
{
  i32 width = window_resize_event.info.width;
  i32 height = window_resize_event.info.height;

  if (window_resize(&app->window, width, height) != D_SUCCESS)
  {
    DFATAL("Error resizing window.");
    return D_FATAL;
  }

  if (renderer_resize(&app->renderer, width, height) != D_SUCCESS)
  {
    DFATAL("Error resizing renderer.");
    return D_FATAL;
  }

  return D_SUCCESS;
}