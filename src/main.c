#include <app.h>
#include <stdio.h>
int main(void)
{
  App app;
  AppCreateInfo info = {"Dorton"};
  app_create(&app, &info); 
  app_run(&app);
  app_destroy(&app);
  getchar();
  return 0;
}