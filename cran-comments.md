
The previous submission to CRAN identified a number of serious
issues with this approach to a PROJ installation. This submission
attempts to rectify the problems that were identified, including

* Rewrite the configure script using autoconf and test it on
  all the platforms supported by rhub
* Update the internal PROJ copy to 8.1.0
* Rewrite tests to make sure the C API can be linked to
  and don't skip them on any platform
* Write an extensive wrapper package that uses most of the
  exported functions to make sure the package is usable
  (see <https://github.com/paleolimbot/rlibproj>)
* Update the README with examples of how to make use of this
  package (optionally using Rcpp or cpp11 as a helper)\
* Correctly declare system requirements.

## Test environments

* local R installation, R 4.1.0
* GitHub Actions (Windows/R-release, MacOS/R-release,
  Ubuntu 16.04, Ubuntu 18.04, Ubuntu 20.04)
* win-builder (devel)
* `rhub::check_for_cran()`
* `rhub::check_on_solaris()`
* `rhub::check_on_macos()`

## R CMD check results

    * checking CRAN incoming feasibility ... NOTE
      Maintainer: ‘Dewey Dunnington <dewey@fishandwhistle.net>’
      
      New submission
      
      Package was archived on CRAN
      
      CRAN repository db overrides:
        X-CRAN-Comment: Archived on 2020-10-05 as serious installation issues
          were not corrected despite multiple notifications.
    
      And multiple broken submissions.
      
    * checking installed package size ... NOTE
        installed size is 13.5Mb
        sub-directories of 1Mb or more:
          libs   6.3Mb
          proj   7.0Mb
    
    0 errors v | 0 warnings v | 2 notes x
