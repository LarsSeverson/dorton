#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if DRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum LOG_LEVEL
{
  FATAL = 0,
  ERROR = 1,
  WARN = 2,
  INFO = 3,
  DEBUG = 4,
  TRACE = 5
} LOG_LEVEL;

DResult init_logging();
void quit_logging();

DAPI void log_print(LOG_LEVEL level, const char *message, ...);

// "##__VA__ARGS" is compiler specific
#define DFATAL(message, ...) log_print(FATAL, message, ##__VA_ARGS__);

// Log an error
#ifndef DERROR
#define DERROR(message, ...) log_print(ERROR, message, ##__VA_ARGS__);
#endif

// Log a warning if warn enabled, else do nothing
#if LOG_WARN_ENABLED == 1
#define DWARN(message, ...) log_print(WARN, message, ##__VA_ARGS__);
#else
#define DWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define DINFO(message, ...) log_print(INFO, message, ##__VA_ARGS__);
#else
#define DINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define DDEBUG(message, ...) log_print(DEBUG, message, ##__VA_ARGS__);
#else
#define DDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define DTRACE(message, ...) log_print(TRACE, message, ##__VA_ARGS__);
#else
#define DTRACE(message, ...)
#endif