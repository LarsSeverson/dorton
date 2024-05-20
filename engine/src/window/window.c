#include "window.h"

#include "core/logger.h"

#include <stdlib.h>

b8 window_create(Window *window, WindowCreateInfo *create_info)
{
  if (window == NULL || create_info == NULL)
  {
    return D_FAILURE;
  }

  window->props = (WindowProperties)*create_info;

  if (!glfwInit()) {
    return D_FAILURE;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window->window = glfwCreateWindow(window->props.width, window->props.height, window->props.title, NULL, NULL);

  if(!window->window) {
    return D_FAILURE;
  }
  return D_SUCCESS;
}

b8 window_destroy(Window *window)
{
  glfwDestroyWindow(window->window);
  glfwTerminate();

  return D_SUCCESS;
}

b8 window_running(Window *window)
{
  return !glfwWindowShouldClose(window->window);
}

b8 window_poll_events(Window *window) {
  glfwPollEvents();

  return D_SUCCESS;
}