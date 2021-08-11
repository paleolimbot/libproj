
<!-- README.md is generated from README.Rmd. Please edit that file -->

# libproj

<!-- badges: start -->

[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
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

You can install the released version of libproj from
[CRAN](https://cran.r-project.org/) with:

``` r
install.packages("libproj")
```

To install a binary on supported Linux platforms, you can use [RStudio
Package Manager’s public instance](https://packagemanager.rstudio.com/)
(see
[here](https://packagemanager.rstudio.com/client/#/repos/1/overview) for
how to configure on your platform).

You can install the development version from [R
Universe](https://r-universe.dev/) with:

``` r
install.packages("libproj", repos = "https://paleolimbot.r-universe.dev")
```

## Configuration

The libproj package provides a configuration that is likely to work on
all systems by default.

``` r
library(libproj)
libproj_configuration()
#> $db_path
#> [1] "/Library/Frameworks/R.framework/Versions/4.1/Resources/library/libproj/proj/proj.db"
#> 
#> $search_path
#> [1] "/Library/Frameworks/R.framework/Versions/4.1/Resources/library/libproj/proj"
#> [2] "/Users/dewey/Library/Application Support/R-libproj/data"                    
#> 
#> $network_endpoint
#> [1] "https://cdn.proj.org"
#> 
#> $log_level
#> [1] 1
#> 
#> $ca_bundle_path
#> [1] NA
#> 
#> $network_enabled
#> [1] FALSE
```

You probably want to install the [PROJ-data
files](https://github.com/OSGeo/PROJ-data) or turn on network access (by
setting `options(libproj.network_enabled = TRUE)` in your .Rprofile) to
ensure that datum transforms are handled correctly. The preferred
approach is to install all the data files (\~600 MB).

``` r
libproj_install_proj_data()
```

    Checking for latest PROJ-data package at https://github.com/OSGeo/PROJ-data/releases/latest
    Downloading 'https://github.com/OSGeo/PROJ-data/releases/download/1.6.0/proj-data-1.6.zip' (558 MB)
    trying URL 'https://github.com/OSGeo/PROJ-data/releases/download/1.6.0/proj-data-1.6.zip'
    Content type 'application/octet-stream' length 585582140 bytes (558.5 MB)
    ==================================================
    downloaded 558.5 MB

    Extracting '/var/folders/bq/2rcjstv90nx1_wrt8d3gqw6m0000gn/T//RtmplU9vQO/filed4072e1fa820.zip' to '/Users/dewey/Library/Application Support/R-libproj/data'
    Saving a record of the install to '/Users/dewey/Library/Application Support/R-libproj/data/.libproj_install_proj_data'
    Done.

## Example

This package only exists for its exported C API. You can use it
interactively (if using Rcpp use `// [[Rcpp::depends(libproj)]]`; if
using cpp11 use `[[cpp11::linking_to(libproj)]]` to decorate at least
one function) or by adding `libproj` to the `LinkingTo` field of a
dependency package.

An example using [cpp11](https://cpp11.r-lib.org):

``` cpp
#include <cpp11.hpp>
using namespace cpp11;

// needed in every file that uses proj_*() functions
#include "libproj.h"

// needed exactly once in your package or Rcpp script
// contains all the function pointers and the
// implementation of the function to initialize them
// (`libproj_init_api()`)
#include "libproj.c"

// this function needs to be called once before any proj_*() functions
// are called (e.g., in .onLoad() for your package)
[[cpp11::linking_to(libproj)]]
[[cpp11::register]]
void cpp_libproj_init_api() {
  libproj_init_api();
}

// regular C or C++ code that uses proj_()* functions!
[[cpp11::register]]
list proj_coords(list xy, std::string from, std::string to) {
  doubles x = xy[0];
  doubles y = xy[1];
  
  PJ_CONTEXT* context = PJ_DEFAULT_CTX;
  
  PJ* trans = proj_create_crs_to_crs(context, from.c_str(), to.c_str(), NULL);
  if (trans == NULL) {
    int error_code = proj_context_errno(context);
    stop("Error creating transform: %s", proj_context_errno_string(context, error_code));
  }
  
  writable::doubles xout(x);
  writable::doubles yout(y);
  size_t stride = sizeof(double);
  
  proj_trans_generic(
    trans, PJ_FWD,
    REAL(xout), stride, xout.size(),
    REAL(yout), stride, yout.size(),
    nullptr, stride, 0,
    nullptr, stride, 0
  );
  
  int error_code = proj_errno(trans);
  proj_destroy(trans);
  
  if (error_code != 0) {
    stop("Error transforming coords: %s", proj_context_errno_string(context, error_code));
  }
  
  writable::list out = {xout, yout};
  out.names() = {"x", "y"};
  return out;
}
```

``` r
cpp_libproj_init_api()
proj_coords(list(-64, 45), "+proj=longlat", "EPSG:32620")
#> $x
#> [1] 421184.7
#> 
#> $y
#> [1] 4983437
```

An example using [Rcpp](https://cran.r-project.org/package=Rcpp):

``` cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::depends(libproj)]]
#include "libproj.h"
#include "libproj.c"

// [[Rcpp::export]]
void cpp_libproj_init_api() {
  libproj_init_api();
}

// [[Rcpp::export]]
List proj_coords(List xy, std::string from, std::string to) {
  NumericVector x = xy[0];
  NumericVector y = xy[1];
  
  PJ_CONTEXT* context = PJ_DEFAULT_CTX;
  
  PJ* trans = proj_create_crs_to_crs(context, from.c_str(), to.c_str(), NULL);
  if (trans == NULL) {
    int errorCode = proj_context_errno(context);
    std::stringstream err;
    err << "Error creating transform: " << proj_context_errno_string(context, errorCode);
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
    err << "Error transforming coords: " << proj_context_errno_string(context, errorCode);
    stop(err.str());
  }
  
  return List::create(_["x"] = xout, _["y"]  = yout);
}
```

``` r
cpp_libproj_init_api()
proj_coords(list(-64, 45), "+proj=longlat", "EPSG:32620")
#> $x
#> [1] 421184.7
#> 
#> $y
#> [1] 4983437
```

You can also link to libproj directly from C:

``` c
#include <R.h>
#include <Rinternals.h>

#include "libproj.h"
#include "libproj.c"

#include <memory.h>

SEXP c_libproj_init_api() {
  libproj_init_api();
  return R_NilValue;
}

SEXP proj_coords(SEXP xy, SEXP from, SEXP to) {
  R_xlen_t n = Rf_xlength(VECTOR_ELT(xy, 0));
  SEXP xout = PROTECT(Rf_allocVector(REALSXP, n));
  SEXP yout = PROTECT(Rf_allocVector(REALSXP, n));

  double* x = REAL(VECTOR_ELT(xy, 0));
  double* y = REAL(VECTOR_ELT(xy, 1));

  const char* from_c = Rf_translateCharUTF8(STRING_ELT(from, 0));
  const char* to_c = Rf_translateCharUTF8(STRING_ELT(to, 0));

  PJ_CONTEXT* context = PJ_DEFAULT_CTX;

  PJ* trans = proj_create_crs_to_crs(context, from_c, to_c, NULL);
  if (trans == NULL) {
    int errorCode = proj_context_errno(context);
    Rf_error("Error creating transform: %s", proj_context_errno_string(context, errorCode));
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
    Rf_error("Error transforming coords: %s", proj_context_errno_string(context, errorCode));
  }

  const char* names[] = {"x", "y", ""};
  SEXP out = PROTECT(Rf_mkNamed(VECSXP, names));
  SET_VECTOR_ELT(out, 0, xout);
  SET_VECTOR_ELT(out, 1, yout);
  UNPROTECT(3);
  
  return out;
}
```

``` r
.Call("c_libproj_init_api")
#> NULL
.Call("proj_coords", list(-64, 45), "+proj=longlat", "EPSG:32620")
#> $x
#> [1] 421184.7
#> 
#> $y
#> [1] 4983437
```
