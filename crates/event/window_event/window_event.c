#include "window_event.h"
#include "logger.h"

#include "app/app.h"

DResult window_resize_event(App *app, WindowEvent event)
{
    app_resize(app, event);

    // DINFO("Window Resize Event: [%d, %d]", event.info.width, event.info.height);

    return D_SUCCESS;
}

DResult window_event_create(WindowEvent *event, WindowEventInfo *window_event_info)
{
    if (event == NULL)
    {
        DERROR("Unable to create window event.");
        return D_ERROR;
    }

    event->info = *window_event_info;

    return D_SUCCESS;
}

void window_resize_event_fn(GLFWwindow *window, i32 width, i32 height)
{
    WindowEventInfo window_resize_event_info = {0};
    window_resize_event_info.width = width;
    window_resize_event_info.height = height;
    window_resize_event_info.event_type = WINDOW_RESIZE_EVENT;
    window_resize_event_info.event_callback_fn = window_resize_event;

    WindowEvent window_event;
    window_event_create(&window_event, &window_resize_event_info);

    App *app = (App*)glfwGetWindowUserPointer(window);

    window_event.info.event_callback_fn(app, window_event);

}