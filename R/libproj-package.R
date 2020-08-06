#' @keywords internal
"_PACKAGE"

# The following block is used by usethis to automatically manage
# roxygen namespace tags. Modify with care!
## usethis namespace: start
#' @useDynLib libproj, .registration = TRUE
## usethis namespace: end
NULL


#' PROJ version information
#'
#' @export
#'
#' @examples
#' libproj_version()
#'
libproj_version <- function() {
  .Call(libproj_proj_version)
}
