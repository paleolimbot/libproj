
test_that("libproj_version works", {
  expect_identical(libproj_version(), "8.1.0")
})

test_that("curl and tiff can be checked", {
  expect_true(libproj_has_libcurl() %in% c(TRUE, FALSE))
  expect_true(libproj_has_libtiff() %in% c(TRUE, FALSE))
})

libproj_source <- function(code) {
  prev_pkg_cpp_flags <- Sys.getenv("PKG_CPPFLAGS")
  code_file <- tempfile(fileext = ".c")
  on.exit({
    Sys.setenv(PKG_CPPFLAGS = prev_pkg_cpp_flags)
    unlink(code_file)
  })

  Sys.setenv(
    PKG_CPPFLAGS = paste0("-I", system.file("include", package = "libproj"))
  )
  writeLines(code, code_file)
  code_file_shell <- gsub("\\\\+", "/", code_file)

  r_exec <- file.path(R.home("bin"), "R")
  system(paste(r_exec, "CMD SHLIB", shQuote(code_file_shell)), ignore.stdout = TRUE)

  shlib_file <- gsub("\\.c$", .Platform$dynlib.ext, code_file)
  dyn.load(shlib_file)
  invisible(shlib_file)
}

test_that("libproj can be linked to", {
  shlib_file <- libproj_source('
  #include "libproj.h"
  #include "libproj.c"
  SEXP libproj_version_run() {
    libproj_init_api();
    return Rf_ScalarInteger(libproj_version_int());
  }
  SEXP libproj_version_build() {
    libproj_init_api();
    return Rf_ScalarInteger(LIBPROJ_VERSION_COMPILE_INT);
  }
  ')

  expect_equal(.Call("libproj_version_run"), 8 * 10000 + 1 * 100 + 0)
  expect_equal(.Call("libproj_version_build"), 8 * 10000 + 1 * 100 + 0)

  unlink(shlib_file)
})

test_that("libproj can project coordinates", {
  shlib_file <- libproj_source('
  #include "libproj.h"
  #include "libproj.c"
  #include <memory.h>

  SEXP libproj_test_proj_coords(SEXP xy, SEXP from, SEXP to) {
    libproj_init_api();

    R_xlen_t n = Rf_xlength(VECTOR_ELT(xy, 0));
    SEXP xout = PROTECT(Rf_allocVector(REALSXP, n));
    SEXP yout = PROTECT(Rf_allocVector(REALSXP, n));

    double* x = REAL(VECTOR_ELT(xy, 0));
    double* y = REAL(VECTOR_ELT(xy, 1));

    const char* from_c = CHAR(STRING_ELT(from, 0));
    const char* to_c = CHAR(STRING_ELT(to, 0));

    PJ_CONTEXT* context = PJ_DEFAULT_CTX;

    PJ* trans = proj_create_crs_to_crs(context, from_c, to_c, NULL);
    if (trans == NULL) {
      int errorCode = proj_context_errno(context);
      Rf_error("Error creating transform: %s", proj_errno_string(errorCode));
    }

    size_t stride = sizeof(double);
    memcpy(REAL(xout), x, n * sizeof(double));
    memcpy(REAL(yout), y, n * sizeof(double));

    proj_trans_generic(
      trans, PJ_FWD,
      REAL(xout), stride, n,
      REAL(yout), stride, n,
      NULL, stride, 0,
      NULL, stride, 0
    );

    int errorCode = proj_errno(trans);
    proj_destroy(trans);

    if (errorCode != 0) {
      // Rf_error("Error transforming coords: %s", proj_errno_string(errorCode));
    }

    SEXP out = PROTECT(Rf_allocVector(VECSXP, 2));
    SET_VECTOR_ELT(out, 0, xout);
    SET_VECTOR_ELT(out, 1, yout);
    UNPROTECT(3);
    return out;
  }
  ')

  proj_coords <- function(x, from, to) {
    .Call("libproj_test_proj_coords", x, from, to)
  }


  # coords <- sf::st_coordinates(sf::st_transform(sf::st_sfc(sf::st_point(c(-64, 45)), crs = 4326), 32620))
  # dput(coords) == c(421184.697083288, 4983436.7683493)
  expect_identical(
    round(
      unlist(proj_coords(list(45, -64), "EPSG:4326", "EPSG:32620"), use.names = FALSE),
      3
    ),
    round(c(421184.697083288, 4983436.7683493), 3)
  )

  # check setting defaults of various lengths to make sure the null-terminated array stuff worked
  libproj_configure(search_path = character(), db_path = rep(system.file("proj/proj.db", package = "libproj"), 2))

  # reset config
  libproj_configure()
  expect_length(proj_coords(list(45, -64), "EPSG:4326", "EPSG:32620"), 2)

  # check setting defaults of various lengths to make sure the null-terminated array stuff worked
  libproj_configure(search_path = c(system.file("proj", package = "libproj"), getwd()))

  # reset config
  libproj_configure()

  if (libproj_has_libcurl() && libproj_has_libtiff()) {
    with_libproj_configuration(list(search_path = libproj_configuration()$search_path[-2], network_enabled = TRUE), {
      # requires a grid
      proj_coords(list(45, -64), "EPSG:4326", "EPSG:26920")
      expect_true("cache.db" %in% list.files(libproj_default_writable_dir()))
    })
  } else if (!libproj_has_libcurl()) {
    expect_error(libproj_configure(network_enabled = TRUE), "Can't enable PROJ network access")
  }

  unlink(shlib_file)
})
