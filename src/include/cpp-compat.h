
#ifndef CPP_COMPAT_H
#define CPP_COMPAT_H

// change to
// #define cpp_compat_assert(expr) assert(expr)
// to turn asserts back on
#define cpp_compat_assert(expr)

#ifdef __cplusplus
extern "C" {
#endif

int cpp_compat_putchar(int c);
void cpp_compat_puts(const char* str);
void cpp_compat_printf(const char* fmt, ...);
void cpp_compat_printerrf(const char* fmt, ...);
void cpp_compat_abort();
void cpp_compat_exit(int code);
long int cpp_compat_random();
void cpp_compat_srandom(int seed);

#ifdef __cplusplus
}
#endif

#endif
