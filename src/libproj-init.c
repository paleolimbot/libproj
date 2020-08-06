
// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

// defined in libproj-version.c
SEXP libproj_proj_version();

static const R_CallMethodDef CallEntries[] = {
  {"libproj_proj_version", (DL_FUNC) &libproj_proj_version, 0},
  {NULL, NULL, 0}
};


void R_init_libproj(DllInfo *dll) {
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
