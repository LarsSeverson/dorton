#include "logger.h"
#include "asserts.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void report_assertion_failure(const char *expression, const char *message, const char *file, i32 line)
{
  log_print(FATAL, "Assertion failture: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}

DResult init_logging()
{
  // TODO: create a log file
  return D_SUCCESS;
}

void quit_logging()
{
  // TODO: cleanup and quit logging
}

void log_print(LOG_LEVEL level, const char *message, ...)
{
  const char *log_levels[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
  // dbool is_error = level < 2;

  // TODO: change this 32000 limit
  const i32 out_limit = 32000;
  char out[out_limit];
  memset(out, 0, sizeof(out));

  // Parse the var args
  __builtin_va_list arg_ptr;
  va_start(arg_ptr, message);
  vsnprintf(out, out_limit, message, arg_ptr);
  va_end(arg_ptr);

  // Append the log level
  char out_final[out_limit];
  snprintf(out_final, out_limit, "%s%s\n", log_levels[level], out);

  // Output
  printf("%s", out_final);
}
