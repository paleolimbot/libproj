
#' Download and install PROJ data files
#'
#' Since PROJ 7.0, the data files used to convert between horizontal and vertical datums
#' are packaged separately. You can access these files as needed using the PROJ
#' networking capability (e.g., with `libproj_configure(network_enabled = TRUE)`) or
#' you can download them all at once. This function searches for the latest release
#' from the [PROJ-data](https://github.com/OSGeo/PROJ-data).
#'
#' @param where The directory into which data files will be installed.
#' @param overwrite Use `NA` to overwrite if newer or `TRUE` to overwrite
#'   completely.
#' @param quiet Use `TRUE` to suppress status messages
#'
#' @return `TRUE` if files were downloaded and installed, `FALSE` otherwise
#' @export
#'
#' @examples
#' # will download ~600 MB!
#' \dontrun{
#' libproj_install_proj_data()
#' }
#'
libproj_install_proj_data <- function(where = libproj_default_data_dir(), overwrite = NA, quiet = FALSE) {
  if (!quiet) message("Checking for latest PROJ-data package at https://github.com/OSGeo/PROJ-data/releases/latest")
  result <- gh::gh("/repos/OSGeo/PROJ-data/releases/latest")

  asset_urls <- vapply(result$assets, "[[", "browser_download_url", FUN.VALUE = character(1))
  zip_asset <- which(grepl("\\.zip$", asset_urls))
  if (length(zip_asset) != 1) {
    stop(
      paste0(
        "Can't determine the .zip release asset for PROJ-data.\n",
        "Try downloading and extracting the .zip file manually to '", where, "'"
      ),
      call. = FALSE
    )
  }

  url <- result$assets[[zip_asset]]$browser_download_url
  size_bytes <- result$assets[[zip_asset]]$size

  record_file <- file.path(where, ".libproj_install_proj_data")
  files_to_clean <- character()

  if (file.exists(record_file)) {
    install_record <- readLines(record_file)
    file_list <- list.files(where)

    if (!isTRUE(overwrite) && identical(install_record[1], paste0(url, "||||", size_bytes))) {
      if (!quiet) message("Installed PROJ-data matches already installed version!\nUse `overwrite = TRUE` to overwrite.")
      if (!quiet) message("Done.")
      return(invisible(FALSE))
    } else if (isTRUE(overwrite)) {
      files_to_clean <- install_record[-1]
    }
  } else if (!isTRUE(overwrite) && (length(list.files(where) > 0))) {
    stop(
      "Use `overwrite = TRUE` to install PROJ data to a non-empty directory.",
      call. = FALSE
    )
  }

  temp_zip <- tempfile(fileext = ".zip")
  on.exit(unlink(temp_zip))
  if (!quiet) message(sprintf("Downloading '%s' (%0.0f MB)", url, size_bytes / (2 ^ 20)))
  utils::download.file(url, temp_zip, mode = "wb", quiet = quiet)

  if (length(files_to_clean) > 0) {
    if (!quiet) message(sprintf("Removing %d previously installed files", length(files_to_clean)))
    unlink(files_to_clean)
  }

  if (!quiet) message(sprintf("Extracting '%s' to '%s'", temp_zip, where))
  utils::unzip(temp_zip, exdir = where, overwrite = isTRUE(overwrite))

  # keep a record of the installed files
  if (!quiet) message(sprintf("Saving a record of the install to '%s'", record_file))
  write(paste0(url, "||||", size_bytes), record_file)
  write(utils::unzip(temp_zip, list = TRUE)[, 1], record_file, append = TRUE)

  if (!quiet) message("Done.")
  invisible(TRUE)
}

#' @rdname libproj_install_proj_data
#' @export
libproj_has_proj_data <- function(where = libproj_default_data_dir()) {
  # better to check an actual file than the install record file
  # in case a user has their own data installed
  check_file <- file.path(where, "us_noaa_README.txt")
  file.exists(check_file)
}
