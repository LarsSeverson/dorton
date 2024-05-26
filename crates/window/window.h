#pragma once

#include "defines.h"

#include <GLFW/glfw3.h>

typedef struct WindowCreateInfo {
  const char *title;
  i32 x;
  i32 y;
  i32 width;
  i32 height;
} WindowCreateInfo, WindowProperties;

typedef struct Window {
  WindowProperties props;
  GLFWwindow *window;
} Window;

DResult window_create(Window* window, WindowCreateInfo* create_info);
DResult window_destroy(Window window);

DAPI b8 window_running(Window* window);
DAPI void window_poll_events(Window* window);
DAPI void window_sleep(u64 ms);


