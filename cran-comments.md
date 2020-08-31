
This update fixes CRAN check errors on MacOS, as requested by the CRAN team.

## Test environments

* local R installation, R 4.0.1
* GitHub Actions (Windows/R-oldrel, Windows/R-release, MacOS/R-release,
  Ubuntu/R-release with curl, Ubuntu/R-release without curl)
* win-builder (devel)
* `rhub::check_for_cran()`
* `rhub::check_on_macos()`

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
