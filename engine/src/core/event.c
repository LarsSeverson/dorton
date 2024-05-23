#include "event.h"
#include "logger.h"

#include <stdlib.h>

DResult mouse_pressed_print_event(MouseEvent event);
DResult mouse_released_print_event(MouseEvent event);

void mouse_event_fn(GLFWwindow *window, int button, int action, int mods) {
  MouseEventCreateInfo create_info= {0};
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

DResult mouse_event_create(MouseEvent *event, MouseEventCreateInfo *create_info)
{
  if (event == NULL)
  {
    DERROR("Unable to create mouse event.");
    return D_ERROR;
  }
  event->info.type = create_info->type;
  event->info.button = create_info->button;
  event->info.action = create_info->action;
  event->info.mods = create_info->mods;

  event->event_callback = create_info->event_callback_fn;

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
