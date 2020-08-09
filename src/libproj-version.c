
#include <Rinternals.h>
#include "proj.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define LIBPROJ_PROJ_VERSION STR(PROJ_VERSION_MAJOR) "." STR(PROJ_VERSION_MINOR) "." STR(PROJ_VERSION_PATCH)

SEXP libproj_proj_version() {
  SEXP out = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(out, 0, Rf_mkChar(LIBPROJ_PROJ_VERSION));
  UNPROTECT(1);
  return out;
}

SEXP libproj_configure_default_context(SEXP dbPath) {
  const char* dbPath0 = CHAR(STRING_ELT(dbPath, 0));
  proj_context_set_database_path(PJ_DEFAULT_CTX, dbPath0, NULL, NULL);

  // by default, the writable directory, which is currently set by env var,
  // is always considered (even if the search paths are empty)
  // the package should be completely isolated from a system install
  // (unless some other package explicitly wants to make this connection)
  proj_context_set_search_paths(PJ_DEFAULT_CTX, 0, NULL);

  // with the default writable directory as a tempdir, it should be safe
  // to enable network by default (other packages should make this choice
  // explicit)
  proj_context_set_enable_network(PJ_DEFAULT_CTX, 1);

  return R_NilValue;
}
