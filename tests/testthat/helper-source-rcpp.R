
source_rcpp_libproj <- function(code, header = '', cache = tempfile(), ..., env = parent.frame()) {
  head <- '
    #include <Rcpp.h>
    // [[Rcpp::depends(libproj)]]
    #include "libproj.h"
    #include "libproj.c"
    using namespace Rcpp;
    // [[Rcpp::export]]
    void source_rcpp_libproj_init() {
      libproj_init_api();
    }
  '
  requireNamespace("libproj", quietly = TRUE)
  if (!dir.exists(cache)) {
    dir.create(cache)
  }

  # compile with C++11 to help with exception handling on Windows i386
  current_flags <- Sys.getenv("PKG_CXXFLAGS")
  Sys.setenv("PKG_CXXFLAGS" = "-std=c++11")
  on.exit(Sys.setenv("PKG_CXXFLAGS" = current_flags))

  Rcpp::sourceCpp(code = paste0(header, '\n\n', head, code), env = env, cacheDir = cache, ...)
  cache
}
