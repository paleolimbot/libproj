
#include "cpp-compat.h"
#include <stdexcept>
#include <stdarg.h>
#include <Rinternals.h>
#include <time.h>
#include <stdio.h>
#include <memory.h>

void cpp_compat_printf(const char* fmt, ...) {
  char buf[8096];
  memset(buf, 0, 8096);
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, 8096, fmt, args);
  va_end(args);

  Rprintf(buf);
}

void cpp_compat_printerrf(const char* fmt, ...) {
  char buf[8096];
  memset(buf, 0, 8096);
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, 8096, fmt, args);
  va_end(args);

  REprintf(buf);
}

void cpp_compat_abort() {
  throw std::runtime_error("abort() called");
}

void cpp_compat_exit(int code) {
  throw std::runtime_error("exit() called");
}

long int cpp_compat_random() {
  // trying to match what random() would return without messing with R's RNG scope
  // this should only be used for (very) low-quality random numbers
  double unif_rand_ish = (clock() % CLOCKS_PER_SEC) / ((double) CLOCKS_PER_SEC);
  return unif_rand_ish * RAND_MAX;
}

void cpp_compat_srandom(int seed) {
  // not used in libproj
}

int cpp_compat_putchar(int c) {
  char out[2];
  out[0] = c;
  out[1] = '\0';
  Rprintf(out);
  return c;
}

void cpp_compat_puts(const char* str) {
  Rprintf(str);
}
