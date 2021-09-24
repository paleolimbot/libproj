
#include <Rinternals.h>
#include "R-libproj/proj.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define LIBPROJ_PROJ_VERSION STR(PROJ_VERSION_MAJOR) "." STR(PROJ_VERSION_MINOR) "." STR(PROJ_VERSION_PATCH)

// Here, the PJ_DEFAULT_CTX is configured. Downstream packages can also
// define their own contexts but this configuration is intended to be
// a reasonable default and can be configured from R (e.g., if a user
// wants to add additional data directories or aux database paths and have
// these choices respected by the rest of the spatial stack).
SEXP libproj_c_configure_default_context(SEXP searchPath, SEXP dbPath, SEXP caPath,
                                         SEXP networkEndpoint, SEXP networkEnabled,
                                         SEXP logLevel) {

  // Set the search paths (this also includes the user-writable directory,
  // which is currently set by environment variable)
  int nSearchPaths = Rf_length(searchPath);
  if (nSearchPaths == 0) {
    proj_context_set_search_paths(PJ_DEFAULT_CTX, 0, NULL);

  } else {
    const char* searchPaths[nSearchPaths];
    for (int i = 0; i < nSearchPaths; i++) {
      searchPaths[i] = Rf_translateCharUTF8(STRING_ELT(searchPath, i));
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
    int setDbSuccess = proj_context_set_database_path(PJ_DEFAULT_CTX, NULL, NULL, NULL);
    if (setDbSuccess == 0) {
      Rf_error("Can't set database path to NULL");
    }

  } else if (nDbPaths == 1) {
    const char* dbPath0 = Rf_translateCharUTF8(STRING_ELT(dbPath, 0));
    int setDbSuccess = proj_context_set_database_path(PJ_DEFAULT_CTX, dbPath0, NULL, NULL);
    if (setDbSuccess == 0) {
      Rf_error("Can't set database path to '%s'", dbPath0);
    }

  } else {
    const char* dbPath0 = Rf_translateCharUTF8(STRING_ELT(dbPath, 0));
    const char* auxPaths[nDbPaths];
    for (int i = 0; i < (nDbPaths - 1); i++) {
      auxPaths[i] = Rf_translateCharUTF8(STRING_ELT(dbPath, i + 1));
    }
    auxPaths[nDbPaths - 1] = NULL;

    int setDbSuccess = proj_context_set_database_path(PJ_DEFAULT_CTX, dbPath0, auxPaths, NULL);
    if (setDbSuccess == 0) {
      Rf_error("Can't set database path to '%s' (or error with one or more aux database paths)", dbPath0);
    }
  }

  // Path to the certificates bundle (for https://)
  const char* caPath0;
  if (STRING_ELT(caPath, 0) == NA_STRING) {
    caPath0 = NULL;
  } else {
    caPath0 = Rf_translateCharUTF8(STRING_ELT(caPath, 0));
  }

  proj_context_set_ca_bundle_path(PJ_DEFAULT_CTX, caPath0);

  // Allow this default to be set from R
  int networkEnabled0 = LOGICAL(networkEnabled)[0];
  int enableNetworkSuccess = proj_context_set_enable_network(PJ_DEFAULT_CTX, networkEnabled0);
  if (enableNetworkSuccess == 0 && networkEnabled0) {
    Rf_error("Can't enable PROJ network access where network access is not available.");
  }

  // The CDN endpoint isn't set by default, and is needed for
  // networking to work out of the box (when enabled by
  // proj_context_set_enable_network(PJ_DEFAULT_CTX, 1)).
  const char* networkEndpoint0 = Rf_translateCharUTF8(STRING_ELT(networkEndpoint, 0));
  proj_context_set_url_endpoint(PJ_DEFAULT_CTX, networkEndpoint0);

  // TODO the default network handler (curl) downloads silently. In the context
  // of an R package, this should really be done with a message indicating
  // when a download takes place. This is not possible without a complex call
  // to proj_set_network_handler() and linking to libcurl, so should probably be implemented
  // here to make it practical for downstream packages to do this as well.

  // Set log level
  int logLevel0 = INTEGER(logLevel)[0];
  proj_log_level(PJ_DEFAULT_CTX, (PJ_LOG_LEVEL) logLevel0);

  // The user writable directory is only solidified when the
  // environment variable is checked. Getting the value forces this
  // check.
  proj_context_get_user_writable_directory(PJ_DEFAULT_CTX, 0);

  return R_NilValue;
}
