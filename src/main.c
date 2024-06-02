#include <app.h>
#include <stdio.h>

int main(void)
{
  App app;
  AppCreateInfo info = {"Dorton"};
  if (app_create(&app, &info) != D_SUCCESS) 
  {
    app_destroy(&app);
    return 1;
  }
  app_run(&app);
  app_destroy(&app);

  return 0;
}