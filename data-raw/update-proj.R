
library(tidyverse)

# ---- PROJ -----

# download PROJ
# check latest release here: https://proj.org/download.html
source_url <- "https://download.osgeo.org/proj/proj-8.1.0.tar.gz"
curl::curl_download(source_url, "data-raw/proj-source.tar.gz")
untar("data-raw/proj-source.tar.gz", exdir = "data-raw")

# make sure the dir exists
proj_dir <- list.files("data-raw", "^proj-[0-9.]+", include.dirs = TRUE, full.names = TRUE)
stopifnot(dir.exists(proj_dir), length(proj_dir) == 1)

# build PROJ! needed for proj_config.h and the db
withr::with_dir(proj_dir, system("./configure"))
withr::with_dir(proj_dir, system("make"))

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
  path = list.files(file.path(proj_dir, "src"), "\\.(h|hpp|c|cpp)$", full.names = TRUE, recursive = TRUE),
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
replace_includes <- . %>%
  str_replace_all('#include\\s+"', '#include "R-libproj/') %>%
  str_replace_all(fixed("<sqlite3.h>"), '"R-libproj/sqlite3.h"')

replace_includes_file <- function(f) {
  content <- read_file(f)
  new_content <- replace_includes(content)
  write_file(new_content, f)
}

c(headers$final_path, source_files$final_path) %>%
  walk(replace_includes_file)

# ---- final steps ----

# need to update objects, because they are in subdirectories
objects <- list.files("src", pattern = "\\.(cpp|c)$", recursive = TRUE, full.names = TRUE) %>%
  gsub("\\.(cpp|c)$", ".o", .) %>%
  gsub("src/", "", .) %>%
  paste(collapse = " \\\n    ")

clipr::write_clip(objects)

#' Manual modifications
#'
#' * Replace stderr/stdout with cpp_compat_printf()/cpp_compat_printerrf()/
#'   cpp_compat_puts()
#'   grep "stdout|stderr" $(find src -name "*.cpp")
#'   grep "stdout|stderr" $(find src -name "*.c")
#'
#' * Replace exit() with cpp_compat_exit()
#'   grep "exit" $(find src -name "*.cpp")
#'   grep "exit" $(find src -name "*.c")
#'
#'  * Replace assert() with cpp_compat_assert()
#'    grep "assert" $(find src -name "*.cpp")
#'    grep "assert" $(find src -name "*.c")
#'
#' * Replace putchar() with cpp_compat_putchar()
#'   grep "putchar" $(find src -name "*.cpp")
#'   grep "putchar" $(find src -name "*.c")
#'
#' * Ensure the first argument to dladdr() is cast to (void\*) and not (const void\*) ",
#'   in filemanager.cpp (required for build to pass on Solaris)"
#'
#' * Remove pragmas supressing diagnostics
#'   grep "pragma" $(find src -name "*.cpp")
#'   grep "pragma" $(find src -name "*.c")
#'
#' * Make sure the variable-length array in proj_internal.h ARG_list does not have
#'   warnings about a flexible-legth member using gcc or clang.s
#'
