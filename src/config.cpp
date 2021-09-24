#include <cpp11.hpp>
using namespace cpp11;

#include "R-libproj/proj.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define LIBPROJ_PROJ_VERSION STR(PROJ_VERSION_MAJOR) "." STR(PROJ_VERSION_MINOR) "." STR(PROJ_VERSION_PATCH)

[[cpp11::register]]
std::string libproj_cpp_version() {
  return LIBPROJ_PROJ_VERSION;
}

[[cpp11::register]]
bool libproj_cpp_has_libtiff() {
#ifdef TIFF_ENABLED
  return true;
#else
  return false;
#endif
}

[[cpp11::register]]
bool libproj_cpp_has_libcurl() {
#ifdef CURL_ENABLED
  return true;
#else
  return false;
#endif
}

[[cpp11::register]]
void libproj_cpp_cleanup() {
  proj_cleanup();
}
