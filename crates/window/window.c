#include "window.h"

#include "../logger/logger.h"
#include "../event/event.h"

#include <stdlib.h>
#include <windows.h>

DResult window_create(Window *window, WindowCreateInfo *create_info)
{
  if (window == NULL || create_info == NULL)
  {
    return D_ERROR;
  }

  window->props = (WindowProperties)*create_info;

  if (!glfwInit())
  {
    return D_ERROR;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window->window = glfwCreateWindow(window->props.width, window->props.height, window->props.title, NULL, NULL);

  if (!window->window)
  {
    return D_ERROR;
  }

  glfwSetMouseButtonCallback(window->window, mouse_event_fn);

  DINFO("Window created successfully.");
  return D_SUCCESS;
}

DResult window_destroy(Window window)
{
  glfwDestroyWindow(window.window);
  glfwTerminate();

  return D_SUCCESS;
}

b8 window_running(Window *window)
{
  return !glfwWindowShouldClose(window->window);
}

void window_poll_events(Window *window)
{
  glfwPollEvents();
}

void window_sleep(u64 ms)
{
  Sleep(ms);
}