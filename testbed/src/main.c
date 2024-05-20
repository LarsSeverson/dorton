#include <window/window.h>
#include <stdio.h>
int main(void)
{
  Window window = {0};

  WindowCreateInfo create_info = {0};
  create_info.height = 720;
  create_info.width = 1250;
  create_info.title = "Dorton";
  window_create(&window, &create_info);
  while(window_running(&window)) {
    window_poll_events(&window);
  }
  window_destroy(&window);
  return 0;
}