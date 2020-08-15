
test_that("libproj_version works", {
  expect_identical(libproj_version(), "7.1.0")
})

test_that("libproj can be linked to", {
  # this test uses sourceRcpp that may generate some errors that are
  # outside the control of this package
  skip_on_cran()

  cache <- source_rcpp_libproj('
    // [[Rcpp::export]]
    List proj_coords(List xy, std::string from, std::string to) {
      NumericVector x = xy[0];
      NumericVector y = xy[1];

      PJ_CONTEXT* context = PJ_DEFAULT_CTX;

      PJ* trans = proj_create_crs_to_crs(context, from.c_str(), to.c_str(), NULL);
      if (trans == NULL) {
        int errorCode = proj_context_errno(context);
        std::stringstream err;
        err << "Error creating transform: " << proj_errno_string(errorCode);
        stop(err.str());
      }

      NumericVector xout = clone(x);
      NumericVector yout = clone(y);
      size_t stride = sizeof(double);

      proj_trans_generic(
        trans, PJ_FWD,
        &(xout[0]), stride, xout.size(),
        &(yout[0]), stride, yout.size(),
        nullptr, stride, 0,
        nullptr, stride, 0
      );

      int errorCode = proj_errno(trans);
      proj_destroy(trans);

      if (errorCode != 0) {
        std::stringstream err;
        err << "Error transforming coords: " << proj_errno_string(errorCode);
        stop(err.str());
      }

      return List::create(_["x"] = xout, _["y"]  = yout);
    }
  ')

  source_rcpp_libproj_init()

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
  expect_length(proj_coords(list(45, -64), "EPSG:4326", "EPSG:32620"), 2)

  # check networking
  expect_identical(list.files(libproj_temp_dir()), character(0))

  if (libproj_has_libcurl() && libproj_has_libtiff()) {
    with_libproj_configuration( list(network_enabled = TRUE), {
      proj_coords(list(45, -64), "EPSG:4326", "EPSG:26920")
    })
    expect_identical(list.files(libproj_temp_dir()), "cache.db")
  }

  unlink(cache, recursive = TRUE)
})
