#include "window.h"

#include "logger.h"

#include "event/event.h"
#include "app/app.h"

#include <stdlib.h>

DResult window_create(App *app, Window *window, WindowCreateInfo *create_info)
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

  window->window_inner = glfwCreateWindow(window->props.width, window->props.height, window->props.title, NULL, NULL);

  if (!window->window_inner)
  {
    return D_ERROR;
  }

  glfwSetWindowUserPointer(window->window_inner, app);

  glfwSetMouseButtonCallback(window->window_inner, mouse_event_fn);
  glfwSetWindowSizeCallback(window->window_inner, window_resize_event_fn);

  DINFO("Window created successfully.");
  return D_SUCCESS;
}

DResult window_destroy(Window *window)
{
  glfwDestroyWindow(window->window_inner);
  glfwTerminate();

  return D_SUCCESS;
}

b8 window_running(Window *window)
{
  return !glfwWindowShouldClose(window->window_inner);
}

void window_poll_events(Window *window)
{
  glfwPollEvents();
}

void window_get_frame_buffer_size(Window *window, i32 *width, i32 *height)
{
  glfwGetFramebufferSize(window->window_inner, width, height);
}