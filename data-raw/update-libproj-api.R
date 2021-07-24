
library(tidyverse)

capi_header <- read_file("src/include/R-libproj/proj.h")

version_defs_chr <- read_lines(capi_header)[173:188]

function_defs_chr <- capi_header %>%
  str_extract_all(
    regex(
      "^[A-Za-z0-9_ ]+\\s*PROJ_DLL[^;]+;",
      multiline = TRUE,
      dotall = TRUE
    )
  ) %>%
  .[[1]] %>%
  str_replace_all(regex("\\s+"), " ")


# too complex for a regex...also includes enums

typedefs_chr <- c(
  read_lines(capi_header)[194:357], # major typedefs to PJ_CONTEXT
  read_lines(capi_header)[369:373], # default context
  read_lines(capi_header)[379:379], # typedef proj_file_finder
  read_lines(capi_header)[389:441], # PROJ_FILE_API
  read_lines(capi_header)[448:511], # PROJ_NETWORK_HANDLE to read range type
  read_lines(capi_header)[577:583], # PJ_DIRECTION
  read_lines(capi_header)[627:651], # error codes
  read_lines(capi_header)[705:1045], # Data types for ISO19111 C API
  read_lines(capi_header)[1068], # PJ_OBJ_LIST
  read_lines(capi_header)[1239], # PJ_INSERT_SESSION
  read_lines(capi_header)[1266] # PJ_OPERATION_FACTORY_CONTEXT
)

function_defs <- tibble(
  # move pointer spec to be with type to the left, remove extern
  def = function_defs_chr %>%
    str_replace("\\s+PROJ_DLL\\s*\\*", "* PROJ_DLL "),

  name = def %>% str_extract("[A-Za-z0-9_]+\\s*\\(") %>% str_remove("\\($") %>% str_trim(),

  return_type = str_extract(def, "^[A-Za-z0-9_ ]+\\*?\\s*PROJ_DLL") %>%
    str_remove("\\s*PROJ_DLL$"),

  args = str_extract(def, "\\([^\\)]*\\)") %>%
    str_remove_all("[\\(\\)]") %>%
    str_split("\\s*,\\s*"),

  arg_types = map(args, str_replace, "(\\**)\\s+(\\**)[a-zA-Z0-9_]+$", "\\1\\2"),

  n_args = map_int(args, length)
) %>%
  # filters out pj_release
  filter(!is.na(name))

function_header_defs <- function_defs %>%
  mutate(
    # declare function pointers
    arg_types = map_chr(arg_types, paste0, collapse = ", "),
    pointer_def = glue::glue("{return_type} (*{name})({arg_types})"),
    header_def = glue::glue("extern {pointer_def};"),
    impl_def = glue::glue("{pointer_def} = NULL;"),
    init_def = glue::glue('  {name} = ({return_type} (*)({arg_types})) R_GetCCallable("libproj", "{name}");'),
    register_def = glue::glue('    R_RegisterCCallable("libproj", "{name}", (DL_FUNC) &{name});')
  )

# make files

libproj_h <- with(
  rlang::list2(
    !!!function_header_defs,
    typedefs_chr = typedefs_chr,
    version_defs_chr = version_defs_chr
  ),
  glue::glue(
    '
// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#ifndef LIBPROJ_H
#define LIBPROJ_H

#ifndef __cplusplus
# include <stddef.h> /* for size_t definition */
#else
# include <cstddef>
using std::size_t;
#endif

#ifdef __cplusplus
extern "C" {{
#endif

#define PROJ_DLL

{ paste0(version_defs_chr, collapse = "\n") }

// how integer versions are calculated
#define LIBPROJ_VERSION_INT(major, minor, patch) (patch + minor * 100 + major * 10000)

// the runtime version of libproj
extern int (*libproj_version_int)();

// the compile-time version of libPROJ
#define LIBPROJ_VERSION_COMPILE_INT LIBPROJ_VERSION_INT(PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH)

{ paste0(typedefs_chr, collapse = "\n") }

{ paste0(header_def, collapse = "\n") }

void libproj_init_api();

#ifdef __cplusplus
}}
#endif

#endif

'
  )
)


libproj_c <- with(
  function_header_defs,
  glue::glue(
    '
// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#include "libproj.h"
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

{ paste0(impl_def, collapse = "\n") }

void libproj_init_api() {{
  libproj_version_int = (int (*)()) R_GetCCallable("libproj", "libproj_version_int");
{ paste0(init_def, collapse = "\n") }
}}

'
  )
)

libproj_init_c <- with(
  function_header_defs,
  glue::glue(
    '
// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include "R-libproj/proj.h"

// we need a utility function to get the runtime version in a form that is
// queryable from the inst/include/libproj.c, because future PROJ versions
// will add to the C API. The ability to do a runtime check around R_GetCCallable()
// lets newer packages link to multiple versions of libproj.
#define LIBPROJ_VERSION_INT(major, minor, patch) (patch + minor * 100 + major * 10000)

int libproj_version_int() {{
  return LIBPROJ_VERSION_INT(PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH);
}}

// defined in libproj-config.c
SEXP libproj_c_version();
SEXP libproj_c_configure_default_context(SEXP searchPath, SEXP dbPath, SEXP caPath,
                                        SEXP networkEndpoint, SEXP networkEnabled);
SEXP libproj_c_has_libtiff();
SEXP libproj_c_has_libcurl();

static const R_CallMethodDef CallEntries[] = {{
  {{"libproj_c_version", (DL_FUNC) &libproj_c_version, 0}},
  {{"libproj_c_has_libtiff", (DL_FUNC) &libproj_c_has_libtiff, 0}},
  {{"libproj_c_has_libcurl", (DL_FUNC) &libproj_c_has_libcurl, 0}},
  {{"libproj_c_configure_default_context", (DL_FUNC) &libproj_c_configure_default_context, 5}},
  {{NULL, NULL, 0}}
}};

void R_init_libproj(DllInfo *dll) {{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);

  /* used by external packages linking to libproj from C */
{ paste0(register_def, collapse = "\n") }
}}

'
  )
)

# check generated code for valid C
tmp <- tempfile()
dir.create(tmp)
write_file(libproj_h, file.path(tmp, "libproj.h"))
write_file(libproj_c, file.path(tmp, "libproj.c"))
write_file(libproj_init_c, file.path(tmp, "libproj-init.c"))

processx::run(
  "clang",
  args = c(
    "-I", "/Library/Frameworks/R.framework/Resources/include",
    "-c",
    "-o", file.path(tmp, "libproj.o"),
    file.path(tmp, "libproj.c")
  )
)

processx::run(
  "clang",
  args = c(
    "-I", "/Library/Frameworks/R.framework/Resources/include",
    "-I", "src/include",
    "-c",
    "-o", file.path(tmp, "libproj-init.o"),
    file.path(tmp, "libproj-init.c")
  )
)

unlink(tmp, recursive = TRUE)

# write auto-generated files!
write_file(libproj_h, "inst/include/libproj.h")
write_file(libproj_c, "inst/include/libproj.c")
write_file(libproj_init_c, "src/libproj-init.c")
