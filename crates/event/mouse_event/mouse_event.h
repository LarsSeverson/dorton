#pragma once

#include "defines.h"

#include <GLFW/glfw3.h>

typedef enum MouseEventType
{
  MousePress = 0,
  MouseRelease = 1,
  MouseHeld = 2
} MouseEventType;

struct MouseEvent;

typedef struct MouseEventInfo
{
  int button;
  int action;
  int mods;

  MouseEventType type;
  DResult (*event_callback_fn)(struct MouseEvent);
} MouseEventInfo;

typedef struct MouseEvent
{
  MouseEventInfo info;
  DResult (*event_callback)(struct MouseEvent);
} MouseEvent;

DResult mouse_event_create(MouseEvent *event, MouseEventInfo *create_info);

void mouse_event_fn(GLFWwindow *window, int button, int action, int mods);