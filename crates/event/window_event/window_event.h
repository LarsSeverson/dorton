#pragma once

#include "defines.h"
#include <GLFW/glfw3.h>

struct WindowEvent;
struct App;

typedef enum WindowEventType 
{
  WINDOW_RESIZE_EVENT = 0,
  WINDOW_CLOSE_EVENT
} WindowEventType;


typedef struct WindowEventInfo
{
  u32 width;
  u32 height;

  WindowEventType event_type;

  DResult (*event_callback_fn)(struct App*, struct WindowEvent);
} WindowEventInfo;

typedef struct WindowEvent
{
  WindowEventInfo info;
} WindowEvent;

DResult window_event_create(WindowEvent *event, WindowEventInfo *window_event_info);

void window_resize_event_fn(GLFWwindow *window, i32 width, i32 height);