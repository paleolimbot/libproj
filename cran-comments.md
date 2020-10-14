
Fixed issues identified by CRAN reviewers (added "value" section
for `libproj*_()` functions).

On 2020-10-05 I was forwarded a notice that this package was archived 
permanently, which included a copy of correspondence on 2020-09-16 
with a list of installation issues and potential fixes. The email 
address listed was correct; however, I have no record of any 
correspondence from CRAN (Inbox, Trash, Spam, or otherwise). 
I don't have any intention of wasting CRAN's volunteer time,
and was surprised that this was the reaction of a missed email
as I have fixed many problems noted by CRAN promptly.
This release fixes the issues noted and was tested using
GitHub Actions, RHub, and several local installs. The test
suite on GitHub Actions also includes a reverse dependecy 
check of the 'PROJ' package (as does CRAN's submission process).

## Test environments

* local R installation, R 4.0.1
* GitHub Actions (Windows/R-oldrel, Windows/R-release, MacOS/R-release,
  Ubuntu 16.04, Ubuntu 18.04, Ubuntu 20.04/R-release with curl,
  Ubuntu 20.04/R-release without curl)
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
      installed size is 49.8Mb
      sub-directories of 1Mb or more:
        libs  43.0Mb
        proj   6.6Mb
    
    0 errors | 0 warnings | 2 notes
