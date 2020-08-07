
<!-- README.md is generated from README.Rmd. Please edit that file -->

# libproj

<!-- badges: start -->

[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
![R-CMD-check](https://github.com/paleolimbot/libproj/workflows/R-CMD-check/badge.svg)
<!-- badges: end -->

The goal of libgeos is to provide access to the [PROJ](https://proj.org)
C API for high-performance geometry operations within the R package
framework. This package contains a copy of the PROJ library, modified
slightly to eliminate errors on all the platforms checked by CRAN and
GitHub actions. This means you don’t have to install anything (other
than the package) to take advantage of PROJ functions in R. It also
means you don’t need a configure script if you’re writing a package that
needs PROJ functionality (just `LinkingTo: libproj`).

## Installation

You can install the released version of libproj from
[CRAN](https://CRAN.R-project.org) with:

``` r
install.packages("libproj")
```

And the development version from [GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("paleolimbot/libproj")
```

## Example

This package only exists for its exported C API, for which headers are
provided to make calling these functions from Rcpp or another package as
easy and as safe as possible.

``` cpp
#include <Rcpp.h>
// Packages will also need LinkingTo: libgeos
// [[Rcpp::depends(libproj)]]
// needed in every file that uses GEOS functions
#include "libproj.h"
// needed exactly once in your package or Rcpp script
// contains all the function pointers and the
// implementation of the function to initialize them
// (`libproj_init_api()`)
#include "libproj.c"
// this function needs to be called once before any GEOS functions
// are called (e.g., in .onLoad() for your package)
// [[Rcpp::export]]
void cpp_libproj_init_api() {
  libproj_init_api();
}
// regular C or C++ code that uses GEOS functions!
// [[Rcpp::export]]
std::string version() {
  return "7.1.0";
}
```

``` r
cpp_libproj_init_api()
version()
#> [1] "7.1.0"
```
