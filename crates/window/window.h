#pragma once

#include "defines.h"

#include <GLFW/glfw3.h>

typedef struct WindowCreateInfo
{
  const char *title;
  i32 x;
  i32 y;
  i32 width;
  i32 height;
} WindowCreateInfo, WindowProperties;

typedef struct Window
{
  WindowProperties props;
  GLFWwindow *window_inner;
} Window;

struct App;

DResult window_create(struct App* app, Window *window, WindowCreateInfo *create_info);
DResult window_destroy(Window *window);

dbool window_running(Window *window);

void window_poll_events(Window *window);
void window_get_frame_buffer_size(Window *window, i32 *width, i32 *height);

DResult window_resize(Window *window, i32 width, i32 height);
