
#include <Rinternals.h>
#include "proj.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define LIBPROJ_PROJ_VERSION STR(PROJ_VERSION_MAJOR) "." STR(PROJ_VERSION_MINOR) "." STR(PROJ_VERSION_PATCH)

SEXP libproj_c_version() {
  SEXP out = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(out, 0, Rf_mkChar(LIBPROJ_PROJ_VERSION));
  UNPROTECT(1);
  return out;
}

SEXP libproj_c_has_libtiff() {
  SEXP out = PROTECT(Rf_allocVector(LGLSXP, 1));
#ifdef TIFF_ENABLED
  LOGICAL(out)[0] = TRUE;
#else
  LOGICAL(out)[0] = FALSE;
#endif
  UNPROTECT(1);
  return out;
}

SEXP libproj_c_has_libcurl() {
  SEXP out = PROTECT(Rf_allocVector(LGLSXP, 1));
#ifdef CURL_ENABLED
  LOGICAL(out)[0] = TRUE;
#else
  LOGICAL(out)[0] = FALSE;
#endif
  UNPROTECT(1);
  return out;
}

// Here, the PJ_DEFAULT_CTX is configured. Downstream packages can also
// define their own contexts but this configuration is intended to be
// a reasonable default and can be configured from R (e.g., if a user
// wants to add additional data directories or aux database paths and have
// these choices respected by the rest of the spatial stack).
SEXP libproj_c_configure_default_context(SEXP searchPath, SEXP dbPath, SEXP networkEndpoint, SEXP networkEnabled) {

  // Set the search paths (this also includes the user-writable directory,
  // which is currently set by environment variable)
  int nSearchPaths = Rf_length(searchPath);
  if (nSearchPaths == 0) {
    proj_context_set_search_paths(PJ_DEFAULT_CTX, 0, NULL);

  } else {
    const char* searchPaths[nSearchPaths];
    for (int i = 0; i < nSearchPaths; i++) {
      searchPaths[i] = CHAR(STRING_ELT(searchPath, i));
    }

    proj_context_set_search_paths(PJ_DEFAULT_CTX, nSearchPaths, searchPaths);
  }

  // Here, the first element of dbPath is used as the database path; the rest are
  // used as the aux database paths. Using character(0) for dbPath means
  // that PROJ will look in searchPath for the database instead.
  int nDbPaths = Rf_length(dbPath);
  if (nDbPaths == 0) {
    // surprisingly, this doesn't seem to "unset" the default database
    // for this reason, this branch is never called because we check length >= 1
    // from `libproj_configure()`
    proj_context_set_database_path(PJ_DEFAULT_CTX, NULL, NULL, NULL);

  } else if (nDbPaths == 1) {
    const char* dbPath0 = CHAR(STRING_ELT(dbPath, 0));
    proj_context_set_database_path(PJ_DEFAULT_CTX, dbPath0, NULL, NULL);

  } else {
    const char* dbPath0 = CHAR(STRING_ELT(dbPath, 0));
    const char* auxPaths[nDbPaths];
    for (int i = 0; i < (nDbPaths - 1); i++) {
      auxPaths[i] = CHAR(STRING_ELT(dbPath, i + 1));
    }
    auxPaths[nDbPaths - 1] = NULL;

    proj_context_set_database_path(PJ_DEFAULT_CTX, dbPath0, auxPaths, NULL);
  }

  // Allow this default to be set from R
  int networkEnabled0 = LOGICAL(networkEnabled)[0];
  proj_context_set_enable_network(PJ_DEFAULT_CTX, networkEnabled0);

  // The CDN endpoint isn't set by default, and is needed for
  // networking to work out of the box (when enabled by
  // proj_context_set_enable_network(PJ_DEFAULT_CTX, 1)).
  const char* networkEndpoint0 = CHAR(STRING_ELT(networkEndpoint, 0));
  proj_context_set_url_endpoint(PJ_DEFAULT_CTX, networkEndpoint0);

  // TODO the default network handler (curl) downloads silently. In the context
  // of an R package, this should really be done with a message indicating
  // when a download takes place. This is not possible without a complex call
  // to proj_set_network_handler() and linking to libcurl, so should probably be implemented
  // here to make it practical for downstream packages to do this as well.

  return R_NilValue;
}
