#include <core/app.h>
#include <stdio.h>
#include <crates/darray.h>
int main(void)
{
  // App app;
  // AppCreateInfo create_info = {0};

  // app_create(&app, &create_info);
  // app_run(&app);
  // getchar();

  DArray array;

  darray_create(&array, int);

  for (int i = 0; i < 25; ++i)
  {
    darray_push(&array, i); 
  }
  for (int i = 0; i < array.size; ++i)
  {
    printf("%i\n", ((int *)(array.raw_array))[i]);
  }
  darray_destroy(array);

  return 0;
}