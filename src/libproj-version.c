
#include <Rinternals.h>
#include "proj.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define GEOPROJ_PROJ_VERSION STR(PROJ_VERSION_MAJOR) "." STR(PROJ_VERSION_MINOR) "." STR(PROJ_VERSION_PATCH)

SEXP libproj_proj_version() {
  SEXP out = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(out, 0, Rf_mkChar(GEOPROJ_PROJ_VERSION));
  UNPROTECT(1);
  return out;
}
