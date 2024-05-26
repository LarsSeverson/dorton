#include "clock.h"

#include <GLFW/glfw3.h>

f64 clock_get_time()
{
  return glfwGetTime();
}

u64 clock_get_nanoseconds()
{
  return (u64)(glfwGetTime() * 1000000000.0);
}

f64 clock_get_milliseconds()
{
  return glfwGetTime() * 1000.0;
}