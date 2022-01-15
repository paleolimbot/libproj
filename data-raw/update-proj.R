
library(tidyverse)

# ---- PROJ -----

# download PROJ
# check latest release here: https://proj.org/download.html
source_url <- "https://download.osgeo.org/proj/proj-8.2.1.tar.gz"
curl::curl_download(source_url, "data-raw/proj-source.tar.gz")
untar("data-raw/proj-source.tar.gz", exdir = "data-raw")

# make sure the dir exists
proj_dir <- list.files("data-raw", "^proj-[0-9.]+", include.dirs = TRUE, full.names = TRUE)
stopifnot(dir.exists(proj_dir), length(proj_dir) == 1)

# build PROJ! needed for proj_config.h and the db
withr::with_dir(proj_dir, system("./configure"))
withr::with_dir(proj_dir, system("make -j8"))

# remove current inst/proj
unlink("inst/proj", recursive = TRUE)
dir.create("inst/proj")

# copy the resource files into inst/
resource_files <- c("proj.db", "GL27", "ITRF2000", "ITRF2008", "ITRF2014", "nad27", "nad83", "world", "other.extra", "CH", "nad.lst")
file.copy(
  file.path(proj_dir, "data", resource_files),
  file.path("inst/proj", resource_files)
)

# headers shouldn't be included in other packages,
# so put them in /src/proj_include
headers <- tibble(
  path = c(
    list.files(file.path(proj_dir, "include"), "\\.(h|hpp)$", full.names = TRUE, recursive = TRUE),
    list.files(file.path(proj_dir, "src"), "\\.(h|hpp)$", full.names = TRUE, recursive = TRUE)
  ),
  final_path = str_replace(path, ".*?(include|src)/", "src/include/R-libproj/")
)

# put sources in src/proj
source_files <- tibble(
  path = list.files(file.path(proj_dir, "src"), "\\.(c|cpp)$", full.names = TRUE, recursive = TRUE),
  final_path = str_replace(path, ".*?src/", "src/proj/")
) %>%
  filter(!str_detect(path, "/(apps|tests?)/"))

# clean source dir
unlink("src/proj/", recursive = TRUE)
unlink("src/include/R-libproj", recursive = TRUE)

# create destination dirs
dest_dirs <- c(
  headers %>% pull(final_path),
  source_files %>% pull(final_path)
) %>%
  dirname() %>%
  unique()
dest_dirs[!dir.exists(dest_dirs)] %>% sort() %>% walk(dir.create, recursive = TRUE)

# copy source files
stopifnot(
  file.copy(headers$path, headers$final_path),
  file.copy(source_files$path, source_files$final_path)
)

# ---- SQLite ----

# also need SQLite3 sources (this is how RSQLite does it)
# check latest release here: https://www.sqlite.org/download.html
curl::curl_download(
  "https://www.sqlite.org/2021/sqlite-amalgamation-3360000.zip",
  "data-raw/sqlite-source.zip"
)
unzip("data-raw/sqlite-source.zip", exdir = "data-raw")
sqlite_dir <- list.files("data-raw", "^sqlite-amalgamation", include.dirs = TRUE, full.names = TRUE)
stopifnot(dir.exists(sqlite_dir), length(sqlite_dir) == 1)

# only two files!
unlink(c("src/include/R-libproj/sqlite3.h", "src/sqlite3.c"))
file.copy(file.path(sqlite_dir, "sqlite3.h"), "src/include/R-libproj/sqlite3.h")
file.copy(file.path(sqlite_dir, "sqlite3.c"), "src/sqlite3.c")

# I've used a custom include path to keep anything from confusing some system
# PROJ with internal PROJ. This shouldn't be a problem because of the ordering
# of the -I flags but is easy to do automatically and was the source of at least
# one hard-to-track-down bug in the past. This doesn't cover everything
# but is a good start.
replace_include <- function(f, inc_statement) {
  inc <- inc_statement %>%
    str_extract('\".*?\"') %>%
    str_remove_all('"')

  if (file.exists(file.path(dirname(f), inc))) {
    # do nothing
  } else {
    m <- headers$final_path[endsWith(headers$final_path, inc)]
    if (length(m) == 0) {
      warning(
        sprintf(
          "Include '%s' in file '%s' had zero matches in headers", inc, f
        ),
        call. = FALSE
      )
    } else {
      inc <- str_replace(m[length(m)], ".*?R-libproj", "R-libproj")
    }
  }

  sprintf('#include "%s"', inc)
}

replace_includes_file <- function(f) {
  content <- read_file(f)

  new_content <- content %>%
    str_replace_all('#include\\s+".*?"', function(inc) replace_include(f, inc)) %>%
    str_replace_all(fixed("<sqlite3.h>"), '"R-libproj/sqlite3.h"')

  write_file(new_content, f)
}

c(headers$final_path, source_files$final_path) %>%
  walk(replace_includes_file)

# ---- update OBJECTS in Makevars ----

# need to update objects, because they are in subdirectories
objects <- list.files("src", pattern = "\\.(cpp|c)$", recursive = TRUE, full.names = TRUE) %>%
  gsub("\\.(cpp|c)$", ".o", .) %>%
  gsub("src/", "", .) %>%
  paste(collapse = " \\\n    ")

clipr::write_clip(objects)
usethis::edit_file("src/Makevars.in")
usethis::edit_file("src/Makevars.win")

# (check to make sure everything builds and tests pass on all systems)

# ---- fix usage that's against CRAN policy ----

src_files <- c(
  list.files("src/proj", "\\.[^o]+$", full.names = TRUE, recursive = TRUE),
  list.files("src/include/R-libproj", full.names = TRUE, recursive = TRUE)
)

file_has_fprintf <- map_lgl(src_files, ~str_detect(read_file(.), "\\bfprintf\\b"))
file_has_fputs <- map_lgl(src_files, ~str_detect(read_file(.), "\\bfputs\\b"))
file_has_puts <- map_lgl(src_files, ~str_detect(read_file(.), "\\bputs\\b"))
file_has_putchar <- map_lgl(src_files, ~str_detect(read_file(.), "\\bputchar\\b"))
file_has_assert <- map_lgl(src_files, ~str_detect(read_file(.), "\\bassert\\b"))
file_has_exit <- map_lgl(src_files, ~str_detect(read_file(.), "\\bexit\\b"))
file_has_abort <- map_lgl(src_files, ~str_detect(read_file(.), "\\babort\\b"))
file_has_rand <- map_lgl(src_files, ~str_detect(read_file(.), "\\brand(om)?\\s*\\("))

file_needs_cpp_compat <- file_has_fprintf | file_has_puts |
  file_has_putchar | file_has_assert | file_has_exit | file_has_abort |
  file_has_rand

src_files[file_needs_cpp_compat] %>%
  walk(~{
    content <- read_file(.)
    if (!str_detect(content, '#include "cpp-compat\\.h"')) {
      content <- paste0('#include "cpp-compat.h"\n', content)
      write_file(content, .)
    }
  })

src_files[file_has_fprintf] %>%
  walk(~{
    content <- read_file(.)

    content <- str_replace_all(
      content,
      "\\bfprintf\\s*\\(\\s*stderr\\s*,\\s*",
      "cpp_compat_printerrf("
    )

    content <- str_replace_all(
      content,
      "\\bfprintf\\s*\\(\\s*stdout\\s*,\\s*",
      "cpp_compat_printf("
    )

    write_file(content, .)
  })

src_files[file_has_fputs] %>%
  walk(~{
    content <- read_file(.)

    content <- str_replace_all(
      content,
      "\\bfputs\\s*\\(\\s*(.*?),\\s*stdout\\s*\\)",
      "cpp_compat_puts(\\1)"
    )

    write_file(content, .)
  })

src_files[file_has_puts] %>%
  walk(~{
    content <- read_file(.)

    content <- str_replace_all(
      content,
      "\\bputs\\s*\\(\\s*",
      "cpp_compat_puts("
    )

    write_file(content, .)
  })

src_files[file_has_putchar] %>%
  walk(~{
    content <- read_file(.)

    content <- str_replace_all(
      content,
      "\\bputchar\\s*\\(\\s*",
      "cpp_compat_putchar("
    )

    write_file(content, .)
  })

src_files[file_has_assert] %>%
  walk(~{
    content <- read_file(.)
    content <- str_replace_all(content, "\\bassert\\s*\\(", "cpp_compat_assert(")
    write_file(content, .)
  })

src_files[file_has_exit] %>%
  walk(~{
    content <- read_file(.)
    content <- str_replace_all(content, "\\bexit\\s*\\(", "cpp_compat_exit(")
    write_file(content, .)
  })

src_files[file_has_abort] %>%
  walk(~{
    content <- read_file(.)
    content <- str_replace_all(content, "\\b(std::)?abort\\s*\\(", "cpp_compat_abort(")
    write_file(content, .)
  })

src_files[file_has_rand] %>%
  walk(~{
    content <- read_file(.)
    content <- str_replace_all(content, "\\brand(om)?\\s*\\(", "cpp_compat_random(")
    write_file(content, .)
  })

# ---- fix pragmas that are against CRAN policy ----

file_has_pragmas <- map_lgl(src_files, ~str_detect(read_file(.), "#pragma\\s+[A-Za-z]+\\s+diagnostic"))

src_files[file_has_pragmas] %>%
  walk(~{
    content <- read_file(.)
    content <- str_replace_all(
      content,
      "#pragma\\s+[A-Za-z]+\\s+diagnostic",
      "// \\0"
    )
    write_file(content, .)
  })

#' Manual modifications
#'
#' * Make sure the variable-length array in proj_internal.h ARG_list does not have
#'   warnings about a flexible-legth member using gcc or clang (-Wpedantic).
#'   See https://github.com/paleolimbot/libproj/blob/v8.1.0-1/src/include/R-libproj/proj_internal.h#L597-L614
#'
browseURL("https://github.com/paleolimbot/libproj/blob/v8.1.0-1/src/include/R-libproj/proj_internal.h#L597-L614")
usethis::edit_file("src/include/R-libproj/proj_internal.h")
