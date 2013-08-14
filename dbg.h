#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

static char* now() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       *buf = (char*)malloc(80*sizeof(char));
  tstruct = *localtime(&now);
  strftime(buf, 80*sizeof(char)-1, "%F %R:%S", &tstruct);

  return buf;
}

static char* _now;

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) _now = now();fprintf(stderr, "[ERROR] %s (%s:%d: errno: %s) " M "\n", _now, __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); free(_now)

#define log_warn(M, ...) _now = now();fprintf(stderr, "[WARN] %s (%s:%d: errno: %s) " M "\n", _now, __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); free(_now)

#define log_info(M, ...) _now = now();fprintf(stderr, "[INFO] %s (%s:%d) " M "\n", _now, __FILE__, __LINE__, ##__VA_ARGS__); free(_now)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
