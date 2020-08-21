
Please find enclosed the new submission 'libproj' for review and possible inclusion in the CRAN repository. I am aware that (1) there is one large file ('proj/proj.db'), which, if not included, would render the package unusable; and (2) that this package contains an internal copy of the 'PROJ' library. Even though there are older versions of the 'PROJ' library available on the CRAN build and check machines, the ability to install a recent version of 'PROJ' on many platforms is impractical or impossible (as described in the package Description), and makes it difficult to reproduce coordinate transformations between computing environments.

## Test environments

* local R installation, R 4.0.1
* GitHub Actions (Windows/R-oldrel, Windows/R-release, MacOS/R-release,
  Ubuntu/R-release with curl, Ubuntu/R-release without curl)
* win-builder (devel)
* `rhub::check_for_cran()`
* `rhub::check_on_solaris()`

## R CMD check results

* checking CRAN incoming feasibility ... NOTE
Maintainer: ‘Dewey Dunnington <dewey@fishandwhistle.net>’

New submission
* checking installed package size ... NOTE
    installed size is 12.6Mb
    sub-directories of 1Mb or more:
      libs   5.9Mb
      proj   6.6Mb

0 errors | 0 warnings | 2 notes
