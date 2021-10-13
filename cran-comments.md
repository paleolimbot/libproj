
This submission fixes the following check errors on various platforms:

* Updated rwinlibs libraries to work on UCRT
* Properly clean up home directory files that may have been written by tests

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
