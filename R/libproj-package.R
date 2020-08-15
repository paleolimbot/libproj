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
#'   package. You must specify at least one database as part of this configuration;
#'   do not rely on `search_path` to find this value for you.
#' @param user_writable_dir A directory that can safely be written to
#'   by this package. This contains a cache of grid shift files downloaded
#'   from the PROJ CDN at `network_endpoint` if using `with_libproj_network()`.
#' @param network_endpoint A mirror of the PROJ CDN of gridshift files. By default,
#'   this is set to <https://cdn.proj.org>.
#' @param network_enabled Whether or not to download gridshift files on the fly.
#'   This defaults to `FALSE`.
#' @param config A named `list()` with elements used to temporarily override elements of the
#'   current [libproj_configuration()].
#' @param expr An expression to evaluate with the specified state
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
with_libproj_configuration <- function(config, expr) {
  stopifnot(
    is.list(config),
    length(config) == 0 || (!is.null(names(config)) && all(names(config) != ""))
  )

  current_config <- libproj_configuration()
  on.exit(do.call(libproj_configure, current_config))

  new_config <- c(config, current_config)[names(config)]
  do.call(libproj_configure, new_config)

  force(expr)
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
  db_path = getOption("libproj.db_path", system.file("proj/proj.db", package = "libproj")),
  user_writable_dir = getOption("libproj.user_writable_dir", libproj_temp_dir()),
  network_endpoint =  getOption("libproj.network_endpoint", "https://cdn.proj.org"),
  network_enabled = getOption("libproj.network_enabled", FALSE)
) {

  search_path <- as.character(search_path)
  db_path <- as.character(db_path)
  user_writable_dir <- as.character(user_writable_dir)
  network_endpoint <- as.character(network_endpoint)
  network_enabled <- as.logical(network_enabled)

  stopifnot(
    all(dir.exists(search_path)),
    length(db_path) >= 1, all(file.exists(db_path)), all(!dir.exists(db_path)),
    length(user_writable_dir) == 1, !is.na(user_writable_dir),
    dir.exists(user_writable_dir) || (!file.exists(user_writable_dir)),
    length(network_endpoint) == 1, !is.na(network_endpoint),
    length(network_enabled) == 1, !is.na(network_enabled)
  )

  # TODO: handle case where this errors (it shouldn't since we've
  # already checked that the files and directories exist, but we don't
  # check the return values)
  .Call(
    libproj_c_configure_default_context,
    search_path,
    db_path,
    network_endpoint,
    network_enabled
  )

  # this currently has to be set by env var, as there is no
  # proj_set_user_writable_dir() at the C level
  Sys.setenv("PROJ_USER_WRITABLE_DIRECTORY" = user_writable_dir)

  # keep a copy of this, since it can't be accessed from C
  libproj_config$search_path <- search_path
  libproj_config$db_path <- db_path
  libproj_config$user_writable_dir <- user_writable_dir
  libproj_config$network_endpoint <- network_endpoint
  libproj_config$network_enabled <- network_enabled
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

  # safely apply default configuration
  if (inherits(try(libproj_configure()), "try-error")) {
    warning(
      paste0(
        "`libproj_configure()` failed, likely as a result of invalid options().\n",
        "Please run `libproj_configure()` with explicit arguments to fix this error."
      ),
      call. = FALSE
    )
  }
}

# cleanup any files that might have been downloaded on exit
.onUnload <- function(...) {
  unlink(libproj_tempdir_, recursive = TRUE)
}
