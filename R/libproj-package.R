#' @keywords internal
"_PACKAGE"

# The following block is used by usethis to automatically manage
# roxygen namespace tags. Modify with care!
## usethis namespace: start
#' @useDynLib libproj, .registration = TRUE
## usethis namespace: end
NULL


#' PROJ configuration
#'
#' @param search_path A character vector of paths where libproj will
#'   look for resource files (e.g., gridshift, database, init files).
#'   Defaults to the internal database and init files distributed
#'   with the PROJ source.
#' @param db_path Independent of `search_path`, a character vector of
#'   SQLite databases that may contain updated or alternative
#'   coordinate operations from the default proj.db included in this
#'   package. By default, this defers to the search path.
#' @param user_writable_dir A directory that can safely be written to
#'   by this package. This contains a cache of grid shift files downloaded
#'   from the PROJ CDN at `network_endpoint` if using `with_libproj_network()`.
#' @param network_endpoint A mirror of the PROJ CDN of gridshift files. By default,
#'   this is set to <https://cdn.proj.org>.
#' @export
#'
#' @examples
#' libproj_version()
#' libproj_has_libtiff()
#' libproj_has_libcurl()
#' libproj_temp_dir()
#' libproj_configuration()
#'
#'
libproj_version <- function() {
  .Call(libproj_c_version)
}

#' @rdname libproj_version
#' @export
libproj_has_libtiff <- function() {
  .Call(libproj_c_has_libtiff)
}

#' @rdname libproj_version
#' @export
libproj_has_libcurl <- function() {
  .Call(libproj_c_has_libcurl)
}

#' @rdname libproj_version
#' @export
libproj_temp_dir <- function() {
  libproj_tempdir_
}

#' @rdname libproj_version
#' @export
libproj_configuration <- function() {
  as.list(libproj_config)
}

#' @rdname libproj_version
#' @export
libproj_configure <- function(
  search_path = c(system.file("proj", package = "libproj"), getOption("libproj.search_path", NULL)),
  db_path = getOption("libproj.db_path", character(0)),
  user_writable_dir = getOption("libproj.user_writable_dir", libproj_temp_dir()),
  network_endpoint =  getOption("libproj.network_endpoint", "https://cdn.proj.org")
) {

  search_path <- as.character(search_path)
  db_path <- as.character(db_path)
  user_writable_dir <- as.character(user_writable_dir)
  network_endpoint <- as.character(network_endpoint)

  stopifnot(
    all(dir.exists(search_path)),
    all(file.exists(db_path)), all(!dir.exists(db_path)),
    length(user_writable_dir) == 1,
    dir.exists(user_writable_dir) || (!file.exists(user_writable_dir)),
    length(network_endpoint) == 1
  )

  # TODO: handle case where this errors (it shouldn't since we've
  # already checked that the files and directories exist, but we don't
  # check the return values)
  .Call(
    libproj_c_configure_default_context,
    search_path,
    db_path,
    network_endpoint
  )

  # this currently has to be set by env var, as there is no
  # proj_set_user_writable_dir() at the C level
  Sys.setenv("PROJ_USER_WRITABLE_DIRECTORY" = user_writable_dir)

  # keep a copy of this, since it can't be accessed from C
  libproj_config$search_path <- search_path
  libproj_config$db_path <- db_path
  libproj_config$user_writable_dir <- user_writable_dir
  libproj_config$network_endpoint <- network_endpoint
}

# by default, this setup makes sure that anybody using PJ_DEFAULT_CTX
# isn't writing files anywhere they didn't mean to (as per CRAN policy)
libproj_tempdir_ <- NULL

# keep a copy of the configuration
libproj_config <- new.env(parent = emptyenv())

.onLoad <- function(...) {
  # by default, user-writable dir is a tempdir that is cleaned up on exit
  libproj_tempdir_ <<- tempfile()
  dir.create(libproj_tempdir_)

  # apply default configuration
  libproj_configure()
}

# cleanup any files that might have been downloaded on exit
.onUnload <- function(...) {
  unlink(libproj_tempdir_, recursive = TRUE)
}
