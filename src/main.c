#include <app.h>
#include <stdio.h>

#include "../crates/dorton_utils/darray/darray.h"
int main(void)
{
  // App app;
  // AppCreateInfo info = {"Dorton"};
  // app_create(&app, &info);
  // app_run(&app);
  // app_destroy(&app);

  DArray test;
  darray_create(&test, int);
  darray_resize(&test, 2);

  printf("%d", test.size);

  getchar();
  return 0;
}