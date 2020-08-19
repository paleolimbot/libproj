
<!-- README.md is generated from README.Rmd. Please edit that file -->

# libproj

<!-- badges: start -->

[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
![R-CMD-check](https://github.com/paleolimbot/libproj/workflows/R-CMD-check/badge.svg)
<!-- badges: end -->

The goal of libproj is to provide access to the [PROJ](https://proj.org)
C API for high-performance geometry operations within the R package
framework. This package contains a copy of the PROJ library, modified
slightly to eliminate errors on all the platforms checked by CRAN and
GitHub actions. This means you don’t have to install anything (other
than the package) to take advantage of PROJ functions in R. It also
means you don’t need a configure script if you’re writing a package that
needs PROJ functionality (just `LinkingTo: libproj`).

## Installation

You can install the development version from
[GitHub](https://github.com/) with:

``` r
# install.packages("remotes")
remotes::install_github("paleolimbot/libproj")
```

## Example

This package only exists for its exported C API:

``` cpp
#include <Rcpp.h>
using namespace Rcpp;

// Packages will also need LinkingTo: libproj
// [[Rcpp::depends(libproj)]]

// needed in every file that uses proj_*() functions
#include "libproj.h"

// needed exactly once in your package or Rcpp script
// contains all the function pointers and the
// implementation of the function to initialize them
// (`libproj_init_api()`)
#include "libproj.c"

// this function needs to be called once before any proj_*() functions
// are called (e.g., in .onLoad() for your package)
// [[Rcpp::export]]
void cpp_libproj_init_api() {
  libproj_init_api();
}

// regular C or C++ code that uses proj_()* functions!
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
```

``` r
cpp_libproj_init_api()

sf::st_transform(sf::st_sfc(sf::st_point(c(-64, 45)), crs = 4326), 32620)
#> Geometry set for 1 feature 
#> geometry type:  POINT
#> dimension:      XY
#> bbox:           xmin: 421184.7 ymin: 4983437 xmax: 421184.7 ymax: 4983437
#> CRS:            EPSG:32620
#> POINT (421184.7 4983437)

# Note that the PROJ default axis ordering (at the C level)
# is not what you expect!
proj_coords(list(45, -64), "EPSG:4326", "EPSG:32620")
#> $x
#> [1] 421184.7
#> 
#> $y
#> [1] 4983437
```

## Configuration

Users can configure libproj to point at any compatible proj.db file
and/or data directory they would like (either permanently or
temporarily\!):

``` r
library(libproj)

# will download a grid shift file from the PROJ CDN
# to correct WGS84->NAD83
with_libproj_configuration(list(network_enabled = TRUE), {
  proj_coords(list(45, -64), "EPSG:4326", "EPSG:26920")
})
#> $x
#> [1] 421184.9
#> 
#> $y
#> [1] 4983437
```

If enabling network downloads, the cache is stored in a temporary
directory (`libproj_temp_dir()`). You can configure this value to
persist the cache between R sessions using
`options(libproj.user_writable_dir = ...)` in your `.Renviron` or using
`with_libproj_configuration()` or `libproj_configure()` to set this
value temporarily. A useful user writable directory might be
`rappdirs::user_data_dir("R-libproj")`.

``` r
# in .Renviron (`usethis::edit_r_environ()`):
options(
  libproj.user_writable_dir = rappdirs::user_data_dir("R-libproj")
)
```

After R is restarted, libproj will use this value as the default:

``` r
libproj_configuration()
#> $db_path
#> [1] "/Library/Frameworks/R.framework/Versions/4.0/Resources/library/libproj/proj/proj.db"
#> 
#> $search_path
#> [1] "/Library/Frameworks/R.framework/Versions/4.0/Resources/library/libproj/proj"
#> 
#> $network_endpoint
#> [1] "https://cdn.proj.org"
#> 
#> $network_enabled
#> [1] FALSE
#> 
#> $user_writable_dir
#> [1] "/Users/dewey/Library/Application Support/R-libproj"
```
