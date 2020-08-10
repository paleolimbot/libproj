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
#' @export
#'
#' @examples
#' libproj_version()
#' libproj_has_libtiff()
#' libproj_has_libcurl()
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

# by default, this setup makes sure that anybody using PJ_DEFAULT_CTX
# isn't writing files anywhere they didn't mean to (realistically
# this should be cached, but that is outside the scope of this package)
libproj_tempdir <- NULL

.onLoad <- function(...) {
  libproj_tempdir <<- tempfile()
  Sys.setenv(PROJ_USER_WRITABLE_DIRECTORY = libproj_tempdir)
  .Call(libproj_c_configure_default_context, system.file("proj.db", package = "libproj"))
}

# cleanup any files that might have been downloaded on exit
.onUnload <- function(...) {
  unlink(libproj_tempdir, recursive = TRUE)
}
