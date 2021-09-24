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
#' @param ca_bundle_path A directory that contains the certificate bundle when
#'   network is enabled. Can be `NA`.
#' @param network_endpoint A mirror of the PROJ CDN of gridshift files. By default,
#'   this is set to <https://cdn.proj.org>.
#' @param network_enabled Whether or not to download gridshift files on the fly.
#'   This defaults to `FALSE`.
#' @param log_level An integer describing the log level: 0 (none), 1 (error), 2 (debug),
#'   3 (trace), or 4 (tell)
#' @param config A named `list()` with elements used to temporarily override elements of the
#'   current [libproj_configuration()].
#' @param expr An expression to evaluate with the specified state
#' @param restore_previous_on_error Use `FALSE` to skip resetting the previous configuration
#'   if the configuration fails.
#' @export
#'
#' @return
#' - `libproj_version()`: A character vector of the proj release.
#' - `libproj_has_libtiff()`: `TRUE` if built against libtiff, `FALSE` otherwise.
#' - `libproj_has_libcurl()`: `TRUE` if built against curl, `FALSE` otherwise.
#' - `libproj_default_writable_dir()`: A character vector of the path where libproj-specific
#'   tempfiles are written.
#' - `libproj_default_data_dir()`: A character vector of the path where the default PROJ-data
#'   installation is kept.
#' - `libproj_configuration()`: A `list()` of values that can be passed to
#'   `libproj_configure()`.
#' - `with_libproj_configuration()`: The value of `expr`.
#' - `libproj_configure()`: `NULL`, invisibly.
#'
#' @examples
#' libproj_version()
#' libproj_configuration()
#'
libproj_version <- function() {
  libproj_cpp_version()
}

#' @rdname libproj_version
#' @export
libproj_has_libtiff <- function() {
  libproj_cpp_has_libtiff()
}

#' @rdname libproj_version
#' @export
libproj_has_libcurl <- function() {
  libproj_cpp_has_libcurl()
}

#' @rdname libproj_version
#' @export
libproj_default_writable_dir <- function() {
  dir <- path.expand(file.path(rappdirs::user_data_dir("R-libproj"), "writable"))
  if (!dir.exists(dir)) {
    dir.create(dir, recursive = TRUE)
  }
  dir
}

#' @rdname libproj_version
#' @export
libproj_default_data_dir <- function() {
  dir <- path.expand(file.path(rappdirs::user_data_dir("R-libproj"), "data"))
  if (!dir.exists(dir)) {
    dir.create(dir, recursive = TRUE)
  }
  dir
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

  new_config <- c(config, current_config)[intersect(names(config), names(current_config))]
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
warn_for_configuration <- function() {
  config <- libproj::libproj_configuration()
  has_data <- vapply(config$search_path, libproj_has_proj_data, logical(1))
  has_network <- config$network_enabled
  check_message <- getOption("libproj.check_data_installed", TRUE)

  if (!any(has_data) && !has_network && check_message) {
    packageStartupMessage(
      paste0(
        "Package 'libproj' is running without data files installed or network enabled\n",
        "which may lead to unexpected coordinate transforms. Silence this message by:\n",
        "- Running `libproj::libproj_install_proj_data()`\n",
        "- Adding `options(libproj.search_path = \"path/to/proj_data\")` to your .Rprofile\n",
        "- Adding `options(libproj.network_enabled = TRUE)` to your .Rprofile\n",
        "- Adding `options(libproj.check_data_installed = FALSE)` to your .Rprofile\n",
        "...and restarting your R session."
      )
    )
  }
}

#' @rdname libproj_version
#' @export
libproj_configure <- function(
  search_path = c(system.file("proj", package = "libproj"), getOption("libproj.search_path", libproj_default_data_dir())),
  db_path = getOption("libproj.db_path", system.file("proj/proj.db", package = "libproj")),
  ca_bundle_path = NA,
  network_endpoint =  getOption("libproj.network_endpoint", "https://cdn.proj.org"),
  network_enabled = getOption("libproj.network_enabled", FALSE),
  log_level = getOption("libproj.log_level", 1L),
  restore_previous_on_error = TRUE
) {

  search_path <- enc2utf8(search_path)
  db_path <- enc2utf8(db_path)
  ca_bundle_path <- enc2utf8(as.character(ca_bundle_path))
  network_endpoint <- enc2utf8(network_endpoint)
  network_enabled <- as.logical(network_enabled)
  log_level <- as.integer(log_level)

  # this can't be unset, so always use the default until it can be
  user_writable_directory <- libproj_default_writable_dir()

  stopifnot(
    all(dir.exists(search_path)),
    length(db_path) >= 1, all(file.exists(db_path)), all(!dir.exists(db_path)),
    length(ca_bundle_path) == 1, is.na(ca_bundle_path) || dir.exists(ca_bundle_path),
    length(network_endpoint) == 1, !is.na(network_endpoint),
    length(network_enabled) == 1, !is.na(network_enabled),
    length(log_level) == 1, log_level >= 0, log_level <= 4,
    length(user_writable_directory) == 1, dir.exists(user_writable_directory)
  )

  # handle case where this errors (it shouldn't since we've
  # already checked that the files and directories exist, but we also check
  # return values at the C level
  old_config <- as.list(libproj_config)

  # writable dir currently has to be set by env var as there is no
  # proj_set_user_writable_dir() at the C level
  # configure_default_context checks the environment variable, solidifying
  # the value.
  old_writable <- Sys.setenv("PROJ_USER_WRITABLE_DIRECTORY" = user_writable_directory)
  on.exit(Sys.setenv("PROJ_USER_WRITABLE_DIRECTORY" = old_writable))

  tryCatch({
    .Call(
      libproj_c_configure_default_context,
      search_path,
      db_path,
      ca_bundle_path,
      network_endpoint,
      network_enabled,
      log_level
    )
  }, error = function(e) {
    if (restore_previous_on_error) {
      tryCatch(
        do.call(libproj_configure, c(old_config, list(restore_previous_on_error = FALSE))),
        error = function(e) {
          warning("Failed to restore previous configuration after error in libproj_configure()")
        }
      )
    }

    stop(e)
  })

  # keep a copy of this, since it can't be accessed from C
  libproj_config$search_path <- search_path
  libproj_config$db_path <- db_path
  libproj_config$ca_bundle_path <- ca_bundle_path
  libproj_config$network_endpoint <- network_endpoint
  libproj_config$network_enabled <- network_enabled
  libproj_config$log_level <- log_level

  invisible(NULL)
}

# keep a copy of the configuration
libproj_config <- new.env(parent = emptyenv())


#' Clean up resources used by the default context
#'
#' @return NULL, invisibly
#' @export
#'
#' @examples
#' libproj_cleanup()
#'
libproj_cleanup <- function() {
  invisible(libproj_cpp_cleanup())
}


.onLoad <- function(...) {
  # load callables
  .Call(libproj_c_register_c_callables)

  # safely apply default configuration
  if (inherits(try(libproj_configure(restore_previous_on_error = FALSE)), "try-error")) {
    warning(
      paste0(
        "`libproj_configure()` failed, likely as a result of invalid options().\n",
        "Please run `libproj_configure()` with explicit arguments to fix this error."
      ),
      call. = FALSE
    )
  }
}

.onAttach <- function(...) {
  warn_for_configuration()
}

.onUnload <- function(...) {
  libproj_cleanup()
}
