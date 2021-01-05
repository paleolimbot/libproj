
library(tidyverse)

capi_header <- read_file("src/proj_include/proj.h")

version_defs_chr <- read_lines(capi_header)[173:175]

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
  read_lines(capi_header)[182:345],
  read_lines(capi_header)[357:361],
  read_lines(capi_header)[366:367],
  read_lines(capi_header)[379:429],
  read_lines(capi_header)[436:499],
  read_lines(capi_header)[565:571],
  read_lines(capi_header)[665:1000],
  read_lines(capi_header)[1157]
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
#include "R-libproj.h"

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

# write auto-generated files!
write_file(libproj_h, "inst/include/libproj.h")
write_file(libproj_c, "inst/include/libproj.c")
write_file(libproj_init_c, "src/libproj-init.c")
