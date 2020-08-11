
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

// Here, the PJ_DEFAULT_CTX is configured so that quick one-off code
// works out of the box. Realistically, downstream packages should create
// their own context and never update the state of the default context, but
// leaving this unconfigured makes it difficult to make sure that this
// package actually works.
SEXP libproj_c_configure_default_context(SEXP dbPath) {

  // It may cause problems if a database from an existing system installation
  // is used by default: if downstream packages want this behaviour, they should
  // explicitly make this connection
  const char* dbPath0 = CHAR(STRING_ELT(dbPath, 0));
  proj_context_set_database_path(PJ_DEFAULT_CTX, dbPath0, NULL, NULL);

  // By default, the writable directory, which is currently set by env var,
  // is always considered (even if the search paths are empty)
  // the package should be completely isolated from a system install
  // (unless a downstream package explicitly wants to make this connection)
  proj_context_set_search_paths(PJ_DEFAULT_CTX, 0, NULL);

  // With the default writable directory as a tempdir, it is safe
  // to enable network with the default CDN, but this is probably not what
  // most users expect (downloads reasonably large files silently for
  // some common operations). This can theoretically be set by users
  // of the default context (but should always be reset to FALSE). Again
  // downstream packages should define their own context to avoid this
  // complexity.
  proj_context_set_enable_network(PJ_DEFAULT_CTX, 0);

  // The cdn.proj.org CDN endpoint isn't set by default, and is needed for
  // networking to work out of the box (when enabled by
  // proj_context_set_enable_network(PJ_DEFAULT_CTX, 1)).
  proj_context_set_url_endpoint(PJ_DEFAULT_CTX, "https://cdn.proj.org");

  // TODO the default network handler (curl) downloads silently. In the context
  // of an R package, this should really be done with a message indicating
  // when a download takes place. This is not possible without a complex call
  // to proj_set_network_handler() and linking to libcurl, so should probably be implemented
  // here to make it practical for downstream packages to do this as well.

  return R_NilValue;
}
