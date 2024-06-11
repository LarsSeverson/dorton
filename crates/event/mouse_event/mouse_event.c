#include "mouse_event.h"
#include "logger.h"

#include <stdlib.h>

DResult mouse_pressed_print_event(MouseEvent event);
DResult mouse_released_print_event(MouseEvent event);

void mouse_event_fn(GLFWwindow *window, int button, int action, int mods) {
  MouseEventInfo create_info= {0};
  create_info.button = button;
  create_info.action = action;
  create_info.mods = mods;

  switch(action) {
    case GLFW_PRESS: {
      create_info.type = MousePress;
      create_info.event_callback_fn = mouse_pressed_print_event;
      break;
    }
    case GLFW_RELEASE: {
      create_info.type = MouseRelease;
      create_info.event_callback_fn = mouse_released_print_event;
      break;
    }
  }

  MouseEvent event;
  mouse_event_create(&event, &create_info);

  event.event_callback(event);
}

DResult mouse_event_create(MouseEvent *event, MouseEventInfo *mouse_event_info)
{
  if (event == NULL)
  {
    DERROR("Unable to create mouse event.");
    return D_ERROR;
  }
  event->info.type = mouse_event_info->type;
  event->info.button = mouse_event_info->button;
  event->info.action = mouse_event_info->action;
  event->info.mods = mouse_event_info->mods;

  event->event_callback = mouse_event_info->event_callback_fn;

  return D_SUCCESS;
}

DResult mouse_pressed_print_event(MouseEvent event)
{
  DINFO("Mouse Pressed Event: %i", event.info.button);
  return D_SUCCESS;
}

DResult mouse_released_print_event(MouseEvent event)
{
  DINFO("Mouse Released Event: %i", event.info.button);
  return D_SUCCESS;
}
