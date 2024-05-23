#include "app.h"
#include "logger.h"

static b8 initialized = false; // TODO: [Temp] no more than 1 app at a time

DResult update(App *app, f32 delta_time);
DResult render(App *app, f32 delta_time);

DResult app_create(App *app, AppCreateInfo *app_info)
{
  if (initialized)
  {
    DFATAL("Only 1 app can exist.");
    return D_ERROR;
  }
  if (init_logging() != D_SUCCESS)
  {
    DERROR("Logging initialization failure.");
    return D_ERROR;
  }

  DINFO("A test");

  app->update = update;
  app->render = render;

  WindowCreateInfo create_info = {0};
  create_info.height = 720;
  create_info.width = 1250;
  create_info.title = "Dorton";

  if (window_create(&app->window, &create_info) != D_SUCCESS)
  {
    DFATAL("Error creating window");
    return D_ERROR;
  }

  initialized = true;
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
  while (window_running(&app->window))
  {
    window_poll_events(&app->window);

    if(app->update(app, 0.f) != D_SUCCESS) {
      DFATAL("App update failed.");
      break;
    }

    if(app->render(app, 0.f) != D_SUCCESS) {
      DFATAL("App render failed.");
      break;
    }
  }

  window_destroy(app->window);
  app->window.window = NULL;
  return D_SUCCESS;
}