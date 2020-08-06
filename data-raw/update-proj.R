
library(tidyverse)

# ---- PROJ -----

# download PROJ
source_url <- "https://download.osgeo.org/proj/proj-7.1.0.tar.gz"
curl::curl_download(source_url, "data-raw/proj-source.tar.gz")
untar("data-raw/proj-source.tar.gz", exdir = "data-raw")

# make sure the dir exists
proj_dir <- list.files("data-raw", "^proj-[0-9.]+", include.dirs = TRUE, full.names = TRUE)
stopifnot(dir.exists(proj_dir), length(proj_dir) == 1)

# build PROJ! needed for proj_config.h and the db
withr::with_dir(proj_dir, system("./configure"))
withr::with_dir(proj_dir, system("make"))

# copy the db into inst/
file.copy(file.path(proj_dir, "data/proj.db"), "inst/proj.db")

# headers shouldn't be included in other packages,
# so put them in /src/proj_include
headers <- tibble(
  path = c(
    list.files(file.path(proj_dir, "include"), "\\.(h|hpp)$", full.names = TRUE, recursive = TRUE),
    list.files(file.path(proj_dir, "src"), "\\.(h|hpp)$", full.names = TRUE, recursive = TRUE)
  ),
  final_path = str_replace(path, ".*?(include|src)/", "src/proj_include/")
)

# put sources in src/proj
source_files <- tibble(
  path = list.files(file.path(proj_dir, "src"), "\\.(h|hpp|c|cpp)$", full.names = TRUE, recursive = TRUE),
  final_path = str_replace(path, ".*?src/", "src/proj/")
) %>%
  filter(!str_detect(path, "/(apps|tests?)/"))

# clean source dir
unlink("src/proj/", recursive = TRUE)
unlink("src/proj_include/", recursive = TRUE)

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
curl::curl_download(
  "https://www.sqlite.org/2020/sqlite-amalgamation-3320300.zip",
  "data-raw/sqlite-source.zip"
)
unzip("data-raw/sqlite-source.zip", exdir = "data-raw")
sqlite_dir <- list.files("data-raw", "^sqlite-amalgamation", include.dirs = TRUE, full.names = TRUE)
stopifnot(dir.exists(sqlite_dir), length(sqlite_dir) == 1)

# only two files!
file.copy(file.path(sqlite_dir, "sqlite3.h"), "src/proj_include/sqlite3.h")
file.copy(file.path(sqlite_dir, "sqlite3.c"), "src/sqlite3.c")

# ---- final steps ----

# need to update objects, because they are in subdirectories
objects <- list.files("src", pattern = "\\.(cpp|c)$", recursive = TRUE, full.names = TRUE) %>%
  gsub("\\.(cpp|c)$", ".o", .) %>%
  gsub("src/", "", .) %>%
  paste("    ", ., "\\", collapse = "\n")

print_next <- function() {
  cli::cat_rule("Manual modifications")

  cli::cat_bullet(
    "Replace stderr/stdout with cpp_compat_"
  )
  cli::cat_bullet(
    "Replace stderr with cpp_compat_"
  )
  cli::cat_bullet(
    "Replace abort() with cpp_compat_abort()"
  )
  cli::cat_bullet(
    "Replace exit() with cpp_compat_exit()"
  )
  cli::cat_bullet(
    "Replace putchar() with cpp_compat_"
  )
  cli::cat_bullet(
    "Replace random() with cpp_compat_random() (sqlite3.c)"
  )

  cli::cat_bullet("Update exported C API using update-libproj-api.R")
  cli::cat_bullet("Update OBJECTS in Makevars (copied to cliboard)")
  clipr::write_clip(objects)
}

print_next()
