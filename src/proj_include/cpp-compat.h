
#ifndef CPP_COMPAT_H
#define CPP_COMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

int cpp_compat_putchar(int c);
void cpp_compat_printf(const char* fmt, ...);
void cpp_compat_printerrf(const char* fmt, ...);
void cpp_compat_abort();
void cpp_compat_exit(int code);
int cpp_compat_random();
void cpp_compat_srandom(int seed);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <ostream>
extern std::ostream& cpp_compat_cerr;
extern std::ostream& cpp_compat_cout;
#endif

#endif
