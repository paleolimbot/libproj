
library(tidyverse)

capi_header <- read_file("src/include/R-libproj/proj.h")

capi_header_no_comments <- capi_header %>%
  str_remove_all(regex("/\\*.*?\\*/", dotall = TRUE, multiline = TRUE)) %>%
  str_replace_all("//.*?\n", "\n")

version_defines <- capi_header %>%
  str_extract_all('#define PROJ_VERSION.*[0-9]\\s*\n') %>%
  unlist() %>%
  str_trim() %>%
  str_replace_all("\\s+", " ")

# typedef struct|union|enum SOMETHING;
typedefs <- capi_header_no_comments %>%
  str_extract_all(
    regex("\ntypedef\\s+(union|struct|enum)[^\\{\\}]*?;\\s*\n", dotall = TRUE, multiline = TRUE)
  ) %>%
  unlist() %>%
  str_trim() %>%
  str_replace_all("\\s+", " ")

typedefs_fun <- capi_header_no_comments %>%
  str_extract_all("\ntypedef.*?\\(\\*[A-Za-z0-9_]+\\)[^;]+;") %>%
  unlist() %>%
  str_trim() %>%
  str_replace_all(" +", " ") %>%
  str_replace_all("\\s*?\n", "\n") %>%
  str_replace_all("\n ([^ ])", "\n  \\1")

# (typedef ) (union|struct|enum) MAYBE_SOMETHING {...} MAYBE_SOMETHING;
structs <- capi_header_no_comments %>%
  str_extract_all(
    regex(
      "(typedef\\s+)?(union|struct|enum)[A-Za-z_ \n]*\\{[^\\}]*\\}[^;]*;",
      dotall = TRUE, multiline = TRUE
    )
  ) %>%
  unlist() %>%
  str_trim() %>%
  str_replace_all("\\s*?\n", "\n") %>%
  str_replace_all(" +", " ") %>%
  str_replace_all("\n(\\s+?\n)+", "\n") %>%
  str_replace_all("\n+", "\n") %>%
  str_replace_all("\n\\{", " {") %>%
  str_replace_all("\n ([^ ])", "\n  \\1")


# enum typedefs have to go first because forward declarations aren't
# valid ISO C++. This only affects PJ_DIRECTION, which is the only one
# that isn't typedef enum
enums <- c(
  str_subset(structs, "enum[^\\{]*\\{"),
  str_subset(typedefs, "enum")
)

typedefs <- str_subset(typedefs, "enum", negate = TRUE)
structs <- str_subset(structs, "enum[^\\{]*\\{", negate = TRUE)

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
    register_def = glue::glue('  R_RegisterCCallable("libproj", "{name}", (DL_FUNC) &{name});')
  )

# make files

libproj_h <- with(
  rlang::list2(
    !!!function_header_defs,
    typedefs = typedefs,
    version_defines = version_defines,
    structs = structs
  ),
  glue::glue(
    '
// generated automatically by data-raw/update-libproj-api.R - do not edit by hand!
#ifndef LIBPROJ_H
#define LIBPROJ_H

#ifndef __cplusplus
# include <stddef.h>
#else
# include <cstddef>
using std::size_t;
#endif

#ifdef __cplusplus
extern "C" {{
#endif

#ifdef __cplusplus
#define PJ_DEFAULT_CTX nullptr
#else
#define PJ_DEFAULT_CTX 0
#endif

{ paste0(version_defines, collapse = "\n") }

// how integer versions are calculated
#define LIBPROJ_VERSION_INT(major, minor, patch) (patch + minor * 100 + major * 10000)

// the runtime version of libproj
extern int (*libproj_version_int)();

// the compile-time version of libproj
#define LIBPROJ_VERSION_COMPILE_INT LIBPROJ_VERSION_INT(PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH)

{ paste0(typedefs, collapse = "\n") }
typedef char **PROJ_STRING_LIST;

{ paste0(enums, collapse = "\n\n") }

{ paste0(typedefs_fun, collapse = "\n\n") }

{ paste0(structs, collapse = "\n\n") }

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

int (*libproj_version_int)() = NULL;
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
#include <R.h>
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

void libproj_register_c_callables() {{
  /* used by external packages linking to libproj from C */
  R_RegisterCCallable("libproj", "libproj_version_int", (DL_FUNC) &libproj_version_int);
  { paste0(register_def, collapse = "\n") }
}}

// defined in libproj-config.c
SEXP libproj_c_version();
SEXP libproj_c_has_libtiff();
SEXP libproj_c_has_libcurl();
SEXP libproj_c_cleanup();
SEXP libproj_c_configure_default_context(SEXP searchPath, SEXP dbPath, SEXP caPath,
                                         SEXP networkEndpoint, SEXP networkEnabled,
                                         SEXP logLevel);

static const R_CallMethodDef CallEntries[] = {{
  {{"libproj_c_version", (DL_FUNC) &libproj_c_version, 0}},
  {{"libproj_c_has_libtiff", (DL_FUNC) &libproj_c_has_libtiff, 0}},
  {{"libproj_c_has_libcurl", (DL_FUNC) &libproj_c_has_libcurl, 0}},
  {{"libproj_c_cleanup", (DL_FUNC) &libproj_c_cleanup, 0}},
  {{"libproj_c_configure_default_context", (DL_FUNC) &libproj_c_configure_default_context, 6}},
  {{NULL, NULL, 0}}
};

void R_init_libproj(DllInfo *dll) {{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  libproj_register_c_callables();
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
